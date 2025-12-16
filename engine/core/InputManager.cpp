#include "InputManager.h"

namespace Engine {
namespace Core {
    // Constructor and Destructor
    InputManager::InputManager()
    : m_mouseX(0),m_mouseY(0),m_lastMouseX(0),m_lastMouseY(0) {}
    InputManager::~InputManager() {}

    //Function implementations:
    void InputManager::beginNewFrame(){
        // Clear just pressed/released states
        m_keysJustPressed.clear();
        m_keysJustReleased.clear();
        m_mouseButtonsJustPressed.clear();
        m_mouseButtonsJustReleased.clear();
        // Update last mouse position
        m_lastMouseX = m_mouseX;
        m_lastMouseY = m_mouseY;
    }
    void InputManager::processEvent(const SDL_Event& event){
        switch(event.type){
            case SDL_EVENT_KEY_DOWN:{
                SDL_Scancode scancode = event.key.scancode;
                if(m_keysPressed.find(scancode) == m_keysPressed.end()){
                    m_keysJustPressed.insert(scancode);
                }
                m_keysPressed.insert(scancode);
                break;
            }
            case SDL_EVENT_KEY_UP:{
                SDL_Scancode scancode = event.key.scancode;
                m_keysPressed.erase(scancode);
                m_keysJustReleased.insert(scancode);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:{
                int button = event.button.button;
                if(m_mouseButtonsPressed.find(button) == m_mouseButtonsPressed.end()){
                    m_mouseButtonsJustPressed.insert(button);
                }
                m_mouseButtonsPressed.insert(button);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:{
                int button = event.button.button;
                m_mouseButtonsPressed.erase(button);
                m_mouseButtonsJustReleased.insert(button);
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:{
                m_mouseX = event.motion.x;
                m_mouseY = event.motion.y;
                break;
            }
            default:
                break; // Ignore other events
        }
    }
    bool InputManager::isKeyPressed(SDL_Scancode scancode) const{
        return m_keysPressed.find(scancode) != m_keysPressed.end();
    }
    bool InputManager::isKeyJustPressed(SDL_Scancode scancode) const{
        return m_keysJustPressed.find(scancode) != m_keysJustPressed.end();
    }
    bool InputManager::isKeyJustReleased(SDL_Scancode scancode) const{
        return m_keysJustReleased.find(scancode) != m_keysJustReleased.end();
    }
    bool InputManager::isMouseButtonPressed(int button) const{
        return m_mouseButtonsPressed.find(button) != m_mouseButtonsPressed.end();
    }
    bool InputManager::isMouseButtonJustPressed(int button) const{
        return m_mouseButtonsJustPressed.find(button) != m_mouseButtonsJustPressed.end();
    }
    bool InputManager::isMouseButtonJustReleased(int button) const{
        return m_mouseButtonsJustReleased.find(button) != m_mouseButtonsJustReleased.end();
    }
    void InputManager::getMouseDelta(int& deltaX, int& deltaY) const{
        deltaX = m_mouseX - m_lastMouseX;
        deltaY = m_mouseY - m_lastMouseY;
    }
} // namespace Core
} // namespace Engine