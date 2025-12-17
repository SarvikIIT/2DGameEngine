#pragma once
#include <glad/glad.h>
#include <string>

namespace Engine {
namespace Renderer {

/**
 * @brief Texture class for loading and managing OpenGL textures
 *
 * Handles:
 * - Loading images from disk (PNG, JPG, BMP, etc.)
 * - Creating OpenGL texture objects
 * - Binding textures for rendering
 * - Automatic cleanup
 */
class Texture {
public:
    /**
     * @brief Construct texture from file path
     * @param filepath Path to image file
     */
    Texture(const std::string& filepath);

    /**
     * @brief Destructor - cleanup OpenGL texture
     */
    ~Texture();

    /**
     * @brief Load texture from file
     * @return true if successful
     */
    bool load();

    /**
     * @brief Bind this texture for rendering
     * @param slot Texture slot (0-31), default 0
     */
    void bind(unsigned int slot = 0) const;

    /**
     * @brief Unbind texture
     */
    void unbind() const;

    /**
     * @brief Get OpenGL texture ID
     * @return Texture ID
     */
    GLuint getID() const { return m_textureID; }

    /**
     * @brief Get texture width
     * @return Width in pixels
     */
    int getWidth() const { return m_width; }

    /**
     * @brief Get texture height
     * @return Height in pixels
     */
    int getHeight() const { return m_height; }

    /**
     * @brief Check if texture loaded successfully
     * @return true if loaded
     */
    bool isLoaded() const { return m_loaded; }

private:
    std::string m_filepath;  // Path to image file
    GLuint m_textureID;      // OpenGL texture ID
    int m_width;             // Texture width
    int m_height;            // Texture height
    int m_channels;          // Number of color channels (RGB=3, RGBA=4)
    bool m_loaded;           // Is texture loaded?
};

} // namespace Renderer
} // namespace Engine
