#include "Window.h"

namespace Engine {
namespace Core {

Window::Window(const std::string& title, int width, int height)
    : m_window(nullptr)
    , m_screenSurface(nullptr)
    , m_imageSurface(nullptr)
    , m_title(title)
    , m_width(width)
    , m_height(height)
{
}

Window::~Window() {
    shutdown();
}
bool Window::init() {
    // Initialize SDL
    bool success = true;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        //Add the OpenGL flag to the window creation
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6); // Minor and major version 4.6
        SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE
        ); // Core profile
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Use double buffering to improve smoothness
        m_window = SDL_CreateWindow(
            m_title.c_str(),
            m_width,
            m_height,
            SDL_WINDOW_OPENGL // Changed the original 0 flag to ask SDL to create an OpenGL context
        );

        if (m_window == nullptr) {
            SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
           // m_screenSurface = SDL_GetWindowSurface(m_window); We now use OpenGL for rendering
        }
    }
    return success;
}

bool Window::loadMedia(const std::string& imagePath) {
    bool success = true;
    m_imageSurface = SDL_LoadBMP(imagePath.c_str());
    if (m_imageSurface == nullptr) {
        SDL_Log("Unable to load image %s! SDL Error: %s\n", imagePath.c_str(), SDL_GetError());
        success = false;
    }
    return success;
}

void Window::render() {
    SDL_FillSurfaceRect(m_screenSurface, nullptr, SDL_MapSurfaceRGB(m_screenSurface, 0xFF, 0xFF, 0xFF));
    SDL_BlitSurface(m_imageSurface, nullptr, m_screenSurface, nullptr);
    SDL_UpdateWindowSurface(m_window);
}

void Window::shutdown() {
    if (m_imageSurface) {
        SDL_DestroySurface(m_imageSurface);
        m_imageSurface = nullptr;
    }
    // Destroy window
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    // Quit SDL subsystems
    SDL_Quit();
}
} // namespace Core
} // namespace Engine