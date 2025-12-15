#pragma once
#include <SDL3/SDL.h>

namespace Engine {
namespace Renderer {

/**
 * @brief Renderer manages OpenGL context and rendering operations
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    /**
     * @brief Initialize OpenGL context for a window
     * @param window The SDL window to attach OpenGL to
     * @return true if successful
     */
    bool init(SDL_Window* window);

    /**
     * @brief Shutdown and cleanup OpenGL
     */
    void shutdown();

    /**
     * @brief Begin rendering a frame (clear screen)
     */
    void beginFrame();

    /**
     * @brief End rendering and display the frame
     * @param window The window to swap buffers for
     */
    void endFrame(SDL_Window* window);

    /**
     * @brief Set the background clear color
     * @param r Red (0.0-1.0)
     * @param g Green (0.0-1.0)
     * @param b Blue (0.0-1.0)
     * @param a Alpha (0.0-1.0)
     */
    void setClearColor(float r, float g, float b, float a);

private:
    SDL_GLContext m_glContext;  // OpenGL context
    float m_clearColor[4];      // Background color
};

} // namespace Renderer
} // namespace Engine
