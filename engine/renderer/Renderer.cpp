#include "Renderer.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>

namespace Engine {
namespace Renderer {
    // Constructor and Destructor
    Renderer::Renderer() : m_glContext(nullptr),m_clearColor{0.2f, 0.3f, 0.3f, 1.0f} {}
    Renderer::~Renderer(){
        shutdown();
    }
    // Initialize OpenGL context
    bool Renderer::init(SDL_Window* window){
        //Check for window validity
        if(!window){
            SDL_Log("Renderer::init - Invalid SDL_Window pointer");
            return false;
        }
        // Create OpenGL context
        m_glContext = SDL_GL_CreateContext(window);
        if(!m_glContext){
            SDL_Log("Renderer::init - Failed to create OpenGL context: %s", SDL_GetError());
            return false;
        }
        // Initialize GLAD by taking the function pointer loader from GL and giving it to GLAD
        if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
            SDL_Log("Renderer::init - Failed to initialize GLAD");
            return false;
        }
        else{
            SDL_Log("Renderer::init - GLAD initialized successfully");
            SDL_Log("OpenGL Version: %s", glGetString(GL_VERSION));
            SDL_GL_SetSwapInterval(1); // Enable VSync

            int width, height;
            SDL_GetWindowSize(window, &width, &height);
            glViewport(0, 0, width, height);

            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            return true;
        }
    }
    void Renderer::shutdown(){
        if(m_glContext){
            SDL_GL_DestroyContext(m_glContext);
            m_glContext = nullptr; // free the dangling pointer
        }
    }
    void Renderer::beginFrame(){
        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void Renderer::endFrame(SDL_Window* window){
        SDL_GL_SwapWindow(window);
    }
    void Renderer::setClearColor(float r, float g, float b, float a){
        m_clearColor[0] = r;
        m_clearColor[1] = g;
        m_clearColor[2] = b;
        m_clearColor[3] = a;
    }

}
}
