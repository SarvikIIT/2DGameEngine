#pragma once
#include "../math/Vector2.h"
#include "../math/Matrix4x4.h"

namespace Engine {
namespace Renderer {

/**
 * @brief 2D Camera for viewport control
 */
class Camera2D {
public:
    /**
     * @brief Constructor
     */
    Camera2D(float viewportWidth, float viewportHeight);

    /**
     * @brief Set camera position in world space
     */
    void setPosition(const Math::Vector2& position);

    /**
     * @brief Move camera by delta
     */
    void move(const Math::Vector2& delta);

    /**
     * @brief Set zoom level (1.0 = normal, 2.0 = 2x zoom in)
     */
    void setZoom(float zoom);

    /**
     * @brief Adjust zoom by delta
     */
    void adjustZoom(float delta);

    /**
     * @brief Get camera position
     */
    const Math::Vector2& getPosition() const { return m_position; }

    /**
     * @brief Get current zoom level
     */
    float getZoom() const { return m_zoom; }

    /**
     * @brief Get view matrix for rendering
     */
    Math::Matrix4x4 getViewMatrix() const;

    /**
     * @brief Convert screen coordinates to world coordinates
     */
    Math::Vector2 screenToWorld(const Math::Vector2& screenPos) const;

    /**
     * @brief Convert world coordinates to screen coordinates
     */
    Math::Vector2 worldToScreen(const Math::Vector2& worldPos) const;

    /**
     * @brief Get the visible world bounds (left, top, right, bottom)
     */
    void getViewBounds(float& left, float& top, float& right, float& bottom) const;

    /**
     * @brief Get viewport width
     */
    float getViewportWidth() const { return m_viewportWidth; }

    /**
     * @brief Get viewport height
     */
    float getViewportHeight() const { return m_viewportHeight; }

private:
    Math::Vector2 m_position;
    float m_zoom;
    float m_viewportWidth;
    float m_viewportHeight;
};

} // namespace Renderer
} // namespace Engine
