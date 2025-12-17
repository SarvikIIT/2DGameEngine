#include "Camera2D.h"

namespace Engine {
namespace Renderer {
    // Constructor:
    Camera2D::Camera2D(float viewportWidth, float viewportHeight)
        : m_position(0.0f, 0.0f), m_zoom(1.0f),
          m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight) {}
    // Set camera position
    void Camera2D::setPosition(const Math::Vector2& position) {
        m_position = position;
    }
    // Move camera by delta
    void Camera2D::move(const Math::Vector2& delta) {
        m_position += delta;
    }
    // Set zoom level
    void Camera2D::setZoom(float zoom) {
        m_zoom = zoom;
        if(zoom<0.1f)m_zoom=0.1f;
        if(zoom>10.0f)m_zoom=10.0f;
    }
    // Adjust zoom by delta
    void Camera2D::adjustZoom(float delta) {
        setZoom(m_zoom + delta);
    }
    // Get view matrix
    Math::Matrix4x4 Camera2D::getViewMatrix() const {
        float centerX = m_viewportWidth / 2.0f;
        float centerY = m_viewportHeight / 2.0f;
        Math::Matrix4x4 view = Math::Matrix4x4::translate(centerX, centerY, 0.0f) *
                               Math::Matrix4x4::scale(m_zoom, m_zoom, 1.0f) *
                               Math::Matrix4x4::translate(-m_position.x, -m_position.y, 0.0f) *
                               Math::Matrix4x4::translate(-centerX, -centerY, 0.0f);
        return view;
    }
    Math::Vector2 Camera2D::screenToWorld(const Math::Vector2& screenPos) const {
        float centerX = m_viewportWidth / 2.0f;
        float centerY = m_viewportHeight / 2.0f;
        Math::Vector2 worldPos;
        worldPos.x = (screenPos.x - centerX) / m_zoom + m_position.x;
        worldPos.y = (screenPos.y - centerY) / m_zoom + m_position.y;
        return worldPos;
    }
    Math::Vector2 Camera2D::worldToScreen(const Math::Vector2& worldPos) const {
        float centerX = m_viewportWidth / 2.0f;
        float centerY = m_viewportHeight / 2.0f;
        Math::Vector2 screenPos;
        screenPos.x = (worldPos.x - m_position.x) * m_zoom + centerX;
        screenPos.y = (worldPos.y - m_position.y) * m_zoom + centerY;
        return screenPos;
    }
    void Camera2D::getViewBounds(float& left, float& top, float& right, float& bottom) const {
        float halfWidth = (m_viewportWidth / 2.0f) / m_zoom;
        float halfHeight = (m_viewportHeight / 2.0f) / m_zoom;
        left = m_position.x - halfWidth;
        right = m_position.x + halfWidth;
        top = m_position.y - halfHeight;
        bottom = m_position.y + halfHeight;
    }

} // namespace Renderer
} // namespace Engine