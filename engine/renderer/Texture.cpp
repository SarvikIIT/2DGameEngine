#include "Texture.h"
#include <SDL3/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Engine {
namespace Renderer {

    //Constructor
    Texture::Texture(const std::string& filepath):
        m_filepath (filepath)
       , m_textureID (0)
       , m_width (0)
       , m_height (0)
       , m_channels (0)
       , m_loaded (false)
    {}
    //Destructor
    Texture::~Texture() {
        if (m_textureID != 0) {
            glDeleteTextures(1, &m_textureID);
        }
    }

    //Load texture from file
    bool Texture::load(){
        //Image Data:
        unsigned char* data = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_channels, 0);
        if(!data){
            SDL_Log("Failed to load texture image: %s", m_filepath.c_str());
            return false;
        }

        //Generate OpenGL texture:
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        //Set texture parameters: Standard Things
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Determine format based on channels:
        GLenum format = (m_channels == 4) ? GL_RGBA : GL_RGB; //Vec4 for RGBA, Vec3 for RGB

        //Upload texture to GPU:
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

        //Free CPU memory:
        stbi_image_free(data);

        m_loaded = true;
        return true;
    }

    //Bind texture for rendering
    void Texture::bind(unsigned int slot) const{
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    //Unbind texture
    void Texture::unbind() const{
        glBindTexture(GL_TEXTURE_2D, 0);
    }

} // Namespace Renderer
} // Namespace Engine