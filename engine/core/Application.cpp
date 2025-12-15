#include "Application.h"
#include <SDL3/SDL.h>

namespace Engine {
namespace Core {

Application::Application(const std::string& title, int width, int height)
    : m_window(std::make_unique<Window>(title, width, height))
    , m_running(false)
{
}

Application::~Application() { //Desctructor
    shutdown();
}

bool Application::init() {
    // Initialize the window
    if (!m_window->init()) {
        SDL_Log("Failed to initialize!\n");
        return false;
    }
    // Load media
    if (!m_window->loadMedia("hello-sdl3.bmp")) {
        SDL_Log("Failed to load media!\n");
        return false;
    }
    return true;
}

void Application::run() {
    m_running = true;
    SDL_Event e;
    SDL_zero(e); // Variable initialization to avoid undefined behavior

    while (m_running) {
        // Process events
        processEvents();
        // Render
        m_window->render();
    }
}

void Application::processEvents() {
    SDL_Event e;
    // Get event data
    while (SDL_PollEvent(&e) == true) {
        // If event is quit type
        if (e.type == SDL_EVENT_QUIT) {
            // End the main loop
            m_running = false;
        }
    }
}
void Application::shutdown() {
    if (m_window) {
        m_window->shutdown();
    }
}
} // namespace Core
} // namespace Engine
