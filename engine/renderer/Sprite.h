#pragma once
#include "engine/math/Vector2.h"
#include "engine/math/Vector4.h"
#include "Texture.h"
#include <memory>

namespace Engine {
namespace Renderer {

/**
 * @brief Sprite class for 2D rendering
 *
 * Represents a textured quad that can be drawn on screen.
 * Has position, size, rotation, and color tint.
 */
class Sprite {
public:
    /**
     * @brief Construct a sprite with a texture
     * @param texture Shared pointer to texture
     */
    Sprite(std::shared_ptr<Texture> texture);

    /**
     * @brief Destructor
     */
    ~Sprite();

    // Transform setters
    void setPosition(const Math::Vector2& position) { m_position = position; }
    void setSize(const Math::Vector2& size) { m_size = size; }
    void setRotation(float rotation) { m_rotation = rotation; }
    void setColor(const Math::Vector4& color) { m_color = color; }

    // Transform getters
    const Math::Vector2& getPosition() const { return m_position; }
    const Math::Vector2& getSize() const { return m_size; }
    float getRotation() const { return m_rotation; }
    const Math::Vector4& getColor() const { return m_color; }

    // Texture
    std::shared_ptr<Texture> getTexture() const { return m_texture; }

    /**
     * @brief Get sprite bounds (left, top, right, bottom)
     */
    void getBounds(float& left, float& top, float& right, float& bottom) const {
        left = m_position.x;
        top = m_position.y;
        right = m_position.x + m_size.x;
        bottom = m_position.y + m_size.y;
    }

private:
    std::shared_ptr<Texture> m_texture;
    Math::Vector2 m_position;  // Position in world space
    Math::Vector2 m_size;      // Width and height
    float m_rotation;          // Rotation in radians
    Math::Vector4 m_color;     // Color tint (RGBA, default white)
};

} // namespace Renderer
} // namespace Engine
