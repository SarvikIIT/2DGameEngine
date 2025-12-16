#include "Application.h"
#include <SDL3/SDL.h>

namespace Engine {
namespace Core {

Application::Application(const std::string& title, int width, int height)
    : m_window(std::make_unique<Window>(title, width, height))
    , m_renderer(std::make_unique<Renderer::Renderer>())
    , m_inputManager(std::make_unique<InputManager>())
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
    //Initialize the renderer with the SDL_Window from m_window
    if(m_renderer ->init(m_window->getSDLWindow()) == false){ // Initialize renderer using the Renderer class
        SDL_Log("Failed to initialize Renderer!\n");
        return false;
    }
    return true;
}

void Application::run() {
    m_running = true;
    SDL_Event e;
    SDL_zero(e); // Variable initialization to avoid undefined behavior

    while (m_running) {
        m_inputManager->beginNewFrame(); // Prepare input manager for new frame
        // Process events
        processEvents();
        // Render
        m_renderer->beginFrame();
        m_renderer->endFrame(m_window->getSDLWindow());
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
        // Pass event to input manager
        m_inputManager->processEvent(e);
    }
}
void Application::shutdown() {
    if(m_renderer){
        m_renderer->shutdown();
        m_renderer.reset(); // free the unique_ptr
    }
    if(m_window){
        m_window->shutdown();
        m_window.reset(); // free the unique_ptr
    }
}
} // namespace Core
} // namespace Engine
