#include "Application.h"
#include <SDL3/SDL.h>

namespace Engine {
namespace Core {

Application::Application(const std::string& title, int width, int height)
    : m_window(std::make_unique<Window>(title, width, height))
    , m_renderer(std::make_unique<Renderer::Renderer>())
    , m_spriteRenderer(std::make_unique<Renderer::SpriteRenderer>())
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

    if(!m_spriteRenderer->init(800, 600)){
        SDL_Log("Failed to initialize SpriteRenderer!\n");
        return false;
    }

    m_testTexture = std::make_shared<Renderer::Texture>("assets/test.png");
    if(!m_testTexture->load()){
        SDL_Log("Failed to load test texture!\n");
        return false;
    }

    // Create 1000 test sprites in a grid
    const int spritesPerRow = 40;
    const int numRows = 25;
    const float spriteSize = 16.0f;
    const float spacing = 20.0f;

    for(int row = 0; row < numRows; ++row){
        for(int col = 0; col < spritesPerRow; ++col){
            auto sprite = std::make_unique<Renderer::Sprite>(m_testTexture);
            sprite->setPosition(Math::Vector2(col * spacing, row * spacing));
            sprite->setSize(Math::Vector2(spriteSize, spriteSize));
            m_testSprites.push_back(std::move(sprite));
        }
    }

    SDL_Log("Created %zu sprites for batch testing", m_testSprites.size());

    m_camera = std::make_unique<Renderer::Camera2D>(800.0f, 600.0f);
    m_camera->setPosition(Math::Vector2(400.0f, 300.0f));

    return true;
}

void Application::run() {
    m_running = true;
    SDL_Event e;
    SDL_zero(e);

    Uint64 perfFreq = SDL_GetPerformanceFrequency();
    int frameCount = 0;
    Uint64 fpsTimer = SDL_GetPerformanceCounter();

    while (m_running) {
        m_inputManager->beginNewFrame();
        processEvents();

        // Camera controls
        float cameraSpeed = 5.0f;
        if(m_inputManager->isKeyPressed(SDL_SCANCODE_W)) m_camera->move(Math::Vector2(0.0f, -cameraSpeed));
        if(m_inputManager->isKeyPressed(SDL_SCANCODE_S)) m_camera->move(Math::Vector2(0.0f, cameraSpeed));
        if(m_inputManager->isKeyPressed(SDL_SCANCODE_A)) m_camera->move(Math::Vector2(-cameraSpeed, 0.0f));
        if(m_inputManager->isKeyPressed(SDL_SCANCODE_D)) m_camera->move(Math::Vector2(cameraSpeed, 0.0f));
        if(m_inputManager->isKeyPressed(SDL_SCANCODE_Q)) m_camera->adjustZoom(-0.01f);
        if(m_inputManager->isKeyPressed(SDL_SCANCODE_E)) m_camera->adjustZoom(0.01f);

        // Update camera
        m_spriteRenderer->setView(m_camera->getViewMatrix());

        m_renderer->beginFrame();

        // Batch render all sprites
        m_spriteRenderer->beginBatch();
        for(const auto& sprite : m_testSprites){
            m_spriteRenderer->submitSprite(*sprite);
        }
        m_spriteRenderer->endBatch();

        m_renderer->endFrame(m_window->getSDLWindow());

        frameCount++;
        Uint64 currentTime = SDL_GetPerformanceCounter();
        if(currentTime - fpsTimer >= perfFreq){
            SDL_Log("FPS: %d - Rendering %zu sprites", frameCount, m_testSprites.size());
            frameCount = 0;
            fpsTimer = currentTime;
        }
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
