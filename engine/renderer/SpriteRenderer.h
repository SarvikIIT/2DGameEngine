#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "Sprite.h"
#include "engine/math/Matrix4x4.h"
#include <memory>
#include <vector>

namespace Engine {
namespace Renderer {

/**
 * @brief Sprite renderer for drawing 2D sprites
 */
class SpriteRenderer {
public:
    /**
     * @brief Constructor
     */
    SpriteRenderer();

    /**
     * @brief Destructor - cleanup OpenGL resources
     */
    ~SpriteRenderer();

    /**
     * @brief Initialize the sprite renderer
     */
    bool init(int windowWidth, int windowHeight);

    /**
     * @brief Set the projection matrix
     */
    void setProjection(const Math::Matrix4x4& projection);

    /**
     * @brief Set the view matrix (camera)
     */
    void setView(const Math::Matrix4x4& view);

    /**
     * @brief Draw a single sprite immediately
     */
    void drawSprite(const Sprite& sprite);

    /**
     * @brief Begin batching sprites
     */
    void beginBatch();

    /**
     * @brief Submit a sprite to the batch
     */
    void submitSprite(const Sprite& sprite);

    /**
     * @brief End batching and draw all sprites
     */
    void endBatch();

    /**
     * @brief Enable/disable frustum culling
     */
    void setFrustumCulling(bool enabled) { m_frustumCullingEnabled = enabled; }

    /**
     * @brief Set frustum bounds for culling (left, top, right, bottom)
     */
    void setFrustumBounds(float left, float top, float right, float bottom);

    /**
     * @brief Get culling statistics
     */
    size_t getTotalSprites() const { return m_totalSprites; }
    size_t getCulledSprites() const { return m_culledSprites; }
    size_t getRenderedSprites() const { return m_renderedSprites; }

private:
    std::unique_ptr<Shader> m_shader;
    GLuint m_VAO;
    GLuint m_VBO;
    Math::Matrix4x4 m_projection;

    // Batching configuration
    static constexpr size_t MAX_SPRITES = 10000;
    static constexpr size_t VERTICES_PER_SPRITE = 6;
    static constexpr size_t FLOATS_PER_VERTEX = 4;

    // Batching data
    std::vector<float> m_batchVertices;
    std::vector<const Sprite*> m_batchSprites;
    bool m_batching;

    // Frustum culling
    bool m_frustumCullingEnabled;
    float m_frustumLeft, m_frustumTop, m_frustumRight, m_frustumBottom;

    // Statistics
    size_t m_totalSprites;
    size_t m_culledSprites;
    size_t m_renderedSprites;

    /**
     * @brief Flush the batch to GPU
     */
    void flush();

    /**
     * @brief Check if sprite is inside frustum
     */
    bool isInFrustum(const Sprite& sprite) const;
};

} // namespace Renderer
} // namespace Engine
