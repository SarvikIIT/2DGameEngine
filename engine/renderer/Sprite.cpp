#include "Sprite.h"

namespace Engine {
namespace Renderer {
    Sprite::Sprite(std::shared_ptr<Texture> texture):
        m_texture(texture)
       , m_position(0.0f, 0.0f)
       , m_size(static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()))
       , m_rotation(0.0f)
       , m_color(1.0f, 1.0f, 1.0f, 1.0f) // RGBA (White)
    {}

    Sprite::~Sprite() {
        // Shared pointer will automatically clean up the texture
    }
} // namespace Renderer
} // namespace Engine