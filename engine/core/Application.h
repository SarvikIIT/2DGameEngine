#pragma once
#include "Window.h"
#include <memory>
#include <vector>
#include "../renderer/Renderer.h"
#include "../renderer/SpriteRenderer.h"
#include "../renderer/Camera2D.h"
#include "../renderer/Texture.h"
#include "../renderer/Sprite.h"
#include "InputManager.h"
namespace Engine {
namespace Core {

/**
 * @brief Application class manages the main game loop
 *
 * Responsibilities:
 * - Initialize subsystems (window, etc.)
 * - Run the main game loop
 * - Handle events
 * - Clean up on shutdown
 */
class Application {
public:
    /**
     * @brief Construct a new Application object
     * @param title Window title
     * @param width Window width
     * @param height Window height
     */
    Application(const std::string& title, int width, int height);

    /**
     * @brief Destroy the Application object
     */
    ~Application();

    /**
     * @brief Initialize the application
     * @return true if successful, false otherwise
     */
    bool init();

    /**
     * @brief Run the main game loop - from main.cpp main() function
     */
    void run();

    /**
     * @brief Shutdown the application
     */
    void shutdown();

private:
    /**
     * @brief Process SDL events - from main.cpp event loop
     */
    void processEvents();
    std::unique_ptr<InputManager> m_inputManager; // Input manager for handling input
    std::unique_ptr<Renderer::Renderer> m_renderer; // Renderer for OpenGL rendering
    std::unique_ptr<Renderer::SpriteRenderer> m_spriteRenderer;
    std::unique_ptr<Renderer::Camera2D> m_camera;
    std::unique_ptr<Window> m_window;  // The window object
    std::shared_ptr<Renderer::Texture> m_testTexture;
    std::vector<std::unique_ptr<Renderer::Sprite>> m_testSprites;
    bool m_running;
};

} // namespace Core
} // namespace Engine
