#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <unordered_set>

namespace Engine {
namespace Core {

/**
 * @brief InputManager handles keyboard and mouse input state
 *
 * Design Pattern: Singleton-style manager with state tracking
 *
 * Features:
 * - Tracks key states (pressed, held, released)
 * - Mouse position and button states
 * - Frame-based input updates
 * - Query interface for game systems
 */
class InputManager {
public:
    /**
     * @brief Construct a new Input Manager
     */
    InputManager();

    /**
     * @brief Destroy the Input Manager
     */
    ~InputManager();

    /**
     * @brief Update input state - call once per frame BEFORE event processing
     * This prepares the input manager for the new frame
     */
    void beginNewFrame();

    /**
     * @brief Process an SDL event
     * @param event The SDL event to process
     * Call this for each SDL_Event in your event loop
     */
    void processEvent(const SDL_Event& event);

    // ===== KEYBOARD QUERIES =====

    /**
     * @brief Check if a key is currently held down
     * @param scancode SDL scancode of the key
     * @return true if key is held down
     */
    bool isKeyPressed(SDL_Scancode scancode) const;

    /**
     * @brief Check if a key was just pressed this frame
     * @param scancode SDL scancode of the key
     * @return true if key was pressed this frame (not last frame)
     */
    bool isKeyJustPressed(SDL_Scancode scancode) const;

    /**
     * @brief Check if a key was just released this frame
     * @param scancode SDL scancode of the key
     * @return true if key was released this frame
     */
    bool isKeyJustReleased(SDL_Scancode scancode) const;

    // ===== MOUSE QUERIES =====

    /**
     * @brief Check if a mouse button is currently held down
     * @param button SDL mouse button (SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT, etc.)
     * @return true if button is held down
     */
    bool isMouseButtonPressed(int button) const;

    /**
     * @brief Check if a mouse button was just pressed this frame
     * @param button SDL mouse button
     * @return true if button was pressed this frame
     */
    bool isMouseButtonJustPressed(int button) const;

    /**
     * @brief Check if a mouse button was just released this frame
     * @param button SDL mouse button
     * @return true if button was released this frame
     */
    bool isMouseButtonJustReleased(int button) const;

    /**
     * @brief Get current mouse X position
     * @return int Mouse X coordinate in pixels
     */
    int getMouseX() const { return m_mouseX; }

    /**
     * @brief Get current mouse Y position
     * @return int Mouse Y coordinate in pixels
     */
    int getMouseY() const { return m_mouseY; }

    /**
     * @brief Get mouse position delta since last frame
     * @param deltaX Output parameter for X delta
     * @param deltaY Output parameter for Y delta
     */
    void getMouseDelta(int& deltaX, int& deltaY) const;

private:
    // Keyboard state tracking
    std::unordered_set<SDL_Scancode> m_keysPressed;      // Currently held keys
    std::unordered_set<SDL_Scancode> m_keysJustPressed;  // Keys pressed this frame
    std::unordered_set<SDL_Scancode> m_keysJustReleased; // Keys released this frame

    // Mouse state tracking
    std::unordered_set<int> m_mouseButtonsPressed;      // Currently held buttons
    std::unordered_set<int> m_mouseButtonsJustPressed;  // Buttons pressed this frame
    std::unordered_set<int> m_mouseButtonsJustReleased; // Buttons released this frame

    int m_mouseX;           // Current mouse X position
    int m_mouseY;           // Current mouse Y position
    int m_lastMouseX;       // Previous frame mouse X
    int m_lastMouseY;       // Previous frame mouse Y
};

} // namespace Core
} // namespace Engine
