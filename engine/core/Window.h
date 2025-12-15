#pragma once
#include <SDL3/SDL.h>
#include <string>

namespace Engine {
namespace Core {

/**
 * @brief Window class manages SDL window creation and lifecycle
 *
 * Responsibilities:
 * - SDL initialization
 * - Window creation with OpenGL support
 * - Window destruction and cleanup
 */
class Window {
public:
    /**
     * @brief Construct a new Window object
     * @param title Window title
     * @param width Window width in pixels
     * @param height Window height in pixels
     */
    Window(const std::string& title, int width, int height);

    /**
     * @brief Destroy the Window object
     */
    ~Window();

    /**
     * @brief Initialize SDL and create the window
     * @return true if successful, false otherwise
     */
    bool init();

    /**
     * @brief Shutdown SDL and destroy the window
     */
    void shutdown();

    /**
     * @brief Load media (BMP image) - from main.cpp loadMedia()
     * @param imagePath Path to the BMP image file
     * @return true if successful, false otherwise
     */
    bool loadMedia(const std::string& imagePath);

    /**
     * @brief Render the loaded image - from main.cpp render loop
     */
    void render();

    /**
     * @brief Get the SDL window pointer
     * @return SDL_Window* The underlying SDL window
     */
    SDL_Window* getSDLWindow() const { return m_window; }

    /**
     * @brief Get the window width
     * @return int Window width in pixels
     */
    int getWidth() const { return m_width; }

    /**
     * @brief Get the window height
     * @return int Window height in pixels
     */
    int getHeight() const { return m_height; }

private:
    SDL_Window* m_window;       // The SDL window handle
    SDL_Surface* m_screenSurface;  // The surface contained by the window (from main.cpp gScreenSurface)
    SDL_Surface* m_imageSurface;   // The image we load and show (from main.cpp gHelloWorld)
    std::string m_title;        // Window title
    int m_width;                // Window width
    int m_height;               // Window height
};

} // namespace Core
} // namespace Engine
