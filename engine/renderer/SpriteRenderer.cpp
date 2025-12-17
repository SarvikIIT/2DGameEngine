#include "SpriteRenderer.h"
#include <SDL3/SDL.h>

namespace Engine {
namespace Renderer {

    // Constructor
    SpriteRenderer::SpriteRenderer()
    : m_VAO(0), m_VBO(0),m_batching(false)
    {
        m_batchVertices.reserve(MAX_SPRITES * VERTICES_PER_SPRITE * FLOATS_PER_VERTEX);
        m_batchSprites.reserve(MAX_SPRITES);
    }
    // Destructor
    SpriteRenderer::~SpriteRenderer() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        m_VAO = 0;
        m_VBO = 0;
    }
    bool SpriteRenderer::init(int windowWidth, int windowHeight){
        //Quad vertices (pos + texcoords):
        float vertices[] = {
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        const std::string vertexShader = R"(
#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main(){
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
)";

        const std::string fragmentShader = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main(){
    color = spriteColor * texture(image, TexCoords);
}
)";

        m_shader = std::make_unique<Shader>(vertexShader, fragmentShader);

        Math::Matrix4x4 projection = Math::Matrix4x4::orthographic(
            0.0f, static_cast<float>(windowWidth),
            static_cast<float>(windowHeight), 0.0f,
            -1.0f, 1.0f
        );

        setProjection(projection);
        m_shader->setInt("image", 0);

        return true;
    }

    void SpriteRenderer::drawSprite(const Sprite& sprite){
        m_shader->use();

        Math::Matrix4x4 model = Math::Matrix4x4::scale(sprite.getSize().x, sprite.getSize().y, 1.0f);
        model = Math::Matrix4x4::translate(sprite.getPosition().x, sprite.getPosition().y, 0.0f) * model;

        m_shader->setMat4("model", model.data());
        m_shader->setVec4("spriteColor", sprite.getColor().x, sprite.getColor().y, sprite.getColor().z, sprite.getColor().w);

        sprite.getTexture()->bind(0);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void SpriteRenderer::setProjection(const Math::Matrix4x4& projection){
        m_projection = projection;
        m_shader->use();
        m_shader->setMat4("projection", projection.data());
    }
    void SpriteRenderer::setView(const Math::Matrix4x4& view){
        m_shader->use();
        Math::Matrix4x4 viewProjection = m_projection * view;
        m_shader->setMat4("projection", viewProjection.data());
    }
    //Prepare for batching:
    void SpriteRenderer::beginBatch(){
        m_batching = true;
        m_batchVertices.clear();
        m_batchSprites.clear();
    }
    void SpriteRenderer::submitSprite(const Sprite& sprite){
        if(!m_batching)return;
        if(m_batchSprites.size() >= MAX_SPRITES){
            flush();
        }
        m_batchSprites.push_back(&sprite);
    }
    void SpriteRenderer::endBatch(){
        if(!m_batching)return;
        flush();
        m_batching = false;
    }
    void SpriteRenderer::flush(){
        if(m_batchSprites.empty())return;
        m_batchVertices.clear();

        for(const Sprite* sprite:m_batchSprites){
            Math::Vector2 pos = sprite->getPosition();
            Math::Vector2 size = sprite->getSize();

            float x = pos.x;
            float y = pos.y;
            float w = size.x;
            float h = size.y;

            m_batchVertices.push_back(x);     m_batchVertices.push_back(y + h); m_batchVertices.push_back(0.0f); m_batchVertices.push_back(1.0f);
            m_batchVertices.push_back(x + w); m_batchVertices.push_back(y);     m_batchVertices.push_back(1.0f); m_batchVertices.push_back(0.0f);
            m_batchVertices.push_back(x);     m_batchVertices.push_back(y);     m_batchVertices.push_back(0.0f); m_batchVertices.push_back(0.0f);

            m_batchVertices.push_back(x);     m_batchVertices.push_back(y + h); m_batchVertices.push_back(0.0f); m_batchVertices.push_back(1.0f);
            m_batchVertices.push_back(x + w); m_batchVertices.push_back(y + h); m_batchVertices.push_back(1.0f); m_batchVertices.push_back(1.0f);
            m_batchVertices.push_back(x + w); m_batchVertices.push_back(y);     m_batchVertices.push_back(1.0f); m_batchVertices.push_back(0.0f);
        }
        m_shader->use();

        Math::Matrix4x4 identity = Math::Matrix4x4::identity();
        m_shader->setMat4("model", identity.data());
        m_shader->setVec4("spriteColor", 1.0f, 1.0f, 1.0f, 1.0f);

        m_batchSprites[0]->getTexture()->bind(0);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_batchVertices.size() * sizeof(float), m_batchVertices.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES,0,m_batchSprites.size() * VERTICES_PER_SPRITE);
        glBindVertexArray(0);

        m_batchSprites.clear();

    }



} // namespace Renderer
} // namespace Engine
