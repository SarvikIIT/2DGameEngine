# 2D Game Engine - Architecture Documentation

## System Overview

```
┌─────────────────────────────────────────────────────────┐
│                    APPLICATION LAYER                     │
│                                                          │
│  ┌────────────────────────────────────────────────────┐ │
│  │           Application (Game Loop)                  │ │
│  │  • Initialize all subsystems                       │ │
│  │  • Run main loop (60fps target)                    │ │
│  │  • Coordinate Window, Input, Renderer              │ │
│  └─────┬──────────────┬──────────────┬────────────────┘ │
└────────┼──────────────┼──────────────┼──────────────────┘
         │              │              │
         ▼              ▼              ▼
    ┌────────┐    ┌──────────┐   ┌──────────┐
    │ Window │    │  Input   │   │ Renderer │
    │        │    │ Manager  │   │          │
    └────────┘    └──────────┘   └──────────┘
         │              │              │
         └──────────────┴──────────────┘
                        │
                        ▼
                  ┌──────────┐
                  │   SDL3   │
                  │  OpenGL  │
                  └──────────┘
```

---

## Class Diagram

```
╔════════════════════════════════════════════════════════╗
║              Engine::Core::Application                 ║
╠════════════════════════════════════════════════════════╣
║ + Application(title, width, height)                    ║
║ + init() : bool                                        ║
║ + run() : void                                         ║
║ + shutdown() : void                                    ║
║ - processEvents() : void                               ║
╠════════════════════════════════════════════════════════╣
║ - m_window : unique_ptr<Window>                        ║
║ - m_renderer : unique_ptr<Renderer>                    ║
║ - m_inputManager : unique_ptr<InputManager>            ║
║ - m_running : bool                                     ║
╚═══════════╦════════════════╦═════════════╦═════════════╝
            ║                ║             ║
            ║ owns           ║ owns        ║ owns
            ║                ║             ║
            ▼                ▼             ▼
    ┌───────────────┐ ┌─────────────┐ ┌──────────────┐
    │    Window     │ │InputManager │ │   Renderer   │
    ├───────────────┤ ├─────────────┤ ├──────────────┤
    │+ init()       │ │+ beginNew   │ │+ init()      │
    │+ shutdown()   │ │  Frame()    │ │+ beginFrame()│
    │+ getSDLWindow│ │+ processEvt │ │+ endFrame()  │
    │               │ │+ isKeyPress │ │+ setClear    │
    │               │ │+ getMouseXY │ │  Color()     │
    ├───────────────┤ ├─────────────┤ ├──────────────┤
    │- m_window*    │ │- m_keys     │ │- m_glContext │
    │- m_width      │ │  Pressed    │ │- m_clearColor│
    │- m_height     │ │- m_mouseX/Y │ │              │
    └───────────────┘ └─────────────┘ └──────────────┘
```

---

## Main Game Loop Flow

```
    START
      │
      ▼
┌──────────────────┐
│ Create App       │
│ • Window         │
│ • Renderer       │
│ • InputManager   │
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│ init()           │
│ • Window::init() │
│ • Renderer::init │
└────────┬─────────┘
         │
         ▼
┌──────────────────────────────────────┐
│        MAIN GAME LOOP                │◄─────┐
│  while(m_running) {                  │      │
│                                      │      │
│    ┌────────────────────────────┐   │      │
│    │ 1. INPUT PHASE             │   │      │
│    │ • beginNewFrame()          │   │      │
│    │   (clear just-pressed)     │   │      │
│    │ • processEvents()          │   │      │
│    │   - Poll SDL events        │   │      │
│    │   - Check QUIT             │   │      │
│    │   - Send to InputManager   │   │      │
│    └────────────────────────────┘   │      │
│              │                       │      │
│              ▼                       │      │
│    ┌────────────────────────────┐   │      │
│    │ 2. UPDATE PHASE (future)   │   │      │
│    │ • Physics                  │   │      │
│    │ • Game logic               │   │      │
│    │ • Collision detection      │   │      │
│    └────────────────────────────┘   │      │
│              │                       │      │
│              ▼                       │      │
│    ┌────────────────────────────┐   │      │
│    │ 3. RENDER PHASE            │   │      │
│    │ • beginFrame() - clear     │   │      │
│    │ • Draw sprites (future)    │   │      │
│    │ • endFrame() - swap buffer │   │      │
│    └────────────────────────────┘   │      │
│                                      │      │
│  }  ─────────────────────────────────┤      │
│      if still running                │──────┘
└──────────────┬───────────────────────┘
               │ m_running = false
               ▼
       ┌──────────────┐
       │ shutdown()   │
       │ • Cleanup    │
       └──────────────┘
               │
               ▼
              END
```

---

## Input System Detail

```
  KEYBOARD/MOUSE
        │
        ▼
  ┌──────────┐
  │SDL Events│
  └─────┬────┘
        │
        ▼
┌─────────────────────────────────────┐
│  Application::processEvents()       │
│  ┌───────────────────────────────┐  │
│  │ while(SDL_PollEvent(&e)) {    │  │
│  │   if(e.type == SDL_EVENT_QUIT)│  │
│  │     m_running = false;        │  │
│  │   inputManager->processEvent()│──┼──┐
│  │ }                             │  │  │
│  └───────────────────────────────┘  │  │
└─────────────────────────────────────┘  │
                                         │
         ┌───────────────────────────────┘
         ▼
┌────────────────────────────────────────────┐
│  InputManager::processEvent(event)         │
│  ┌──────────────────────────────────────┐  │
│  │ switch(event.type) {                 │  │
│  │   KEY_DOWN:                          │  │
│  │     if(!already pressed)             │  │
│  │       m_keysJustPressed.insert()     │  │
│  │     m_keysPressed.insert()           │  │
│  │                                      │  │
│  │   KEY_UP:                            │  │
│  │     m_keysPressed.erase()            │  │
│  │     m_keysJustReleased.insert()      │  │
│  │                                      │  │
│  │   MOUSE_MOTION:                      │  │
│  │     m_mouseX = event.motion.x        │  │
│  │     m_mouseY = event.motion.y        │  │
│  │ }                                    │  │
│  └──────────────────────────────────────┘  │
└────────────────────────────────────────────┘
         │
         │ State stored in:
         ▼
┌─────────────────────────────────┐
│  unordered_set<SDL_Scancode>   │
│  • m_keysPressed                │
│  • m_keysJustPressed            │
│  • m_keysJustReleased           │
│                                 │
│  int m_mouseX, m_mouseY         │
└─────────────────────────────────┘
         │
         │ Game queries:
         ▼
┌─────────────────────────────────┐
│  if(input.isKeyPressed(SPACE))  │
│    player.jump();               │
│                                 │
│  if(input.isKeyJustPressed(E))  │
│    interact();                  │
└─────────────────────────────────┘
         │
         ▼
    NEXT FRAME
         │
         ▼
┌─────────────────────────────────┐
│  beginNewFrame()                │
│  • Clear just-pressed sets      │
│  • Update lastMouseX/Y          │
└─────────────────────────────────┘
```

---

## Memory Ownership

```
┌──────────────────────────────┐
│  Application (stack object)  │
│  Created in main.cpp         │
└───────────┬──────────────────┘
            │ owns via unique_ptr
            │
    ┌───────┼───────┬──────────┐
    │       │       │          │
    ▼       ▼       ▼          ▼
┌────────┐┌───────┐┌────────┐
│ Window ││Renderer││ Input  │ (heap)
│        ││       ││Manager │
└───┬────┘└───┬───┘└────────┘
    │         │
    │ owns    │ owns
    ▼         ▼
┌─────────┐┌──────────┐
│SDL      ││OpenGL    │
│Window   ││Context   │
└─────────┘└──────────┘

Rules:
• Application owns subsystems (unique_ptr)
• Automatic cleanup on destruction
• No manual delete needed
• No memory leaks
```

---

## File Structure

```
2D Game Engine/
│
├── engine/
│   ├── core/
│   │   ├── Application.h
│   │   ├── Application.cpp
│   │   ├── Window.h
│   │   ├── Window.cpp
│   │   ├── InputManager.h
│   │   └── InputManager.cpp
│   │
│   └── renderer/
│       ├── Renderer.h
│       └── Renderer.cpp
│
├── main.cpp
├── CMakeLists.txt
└── vcpkg.json
```

---

## Key Design Patterns

### 1. **Manager Pattern**
- InputManager, Window, Renderer are all "manager" objects
- Each handles a specific subsystem
- Application coordinates them

### 2. **RAII (Resource Acquisition Is Initialization)**
- unique_ptr automatically cleans up
- Constructors initialize, destructors cleanup
- No manual memory management

### 3. **Event-Driven Architecture**
- SDL generates events
- Application dispatches to InputManager
- Game queries input state

### 4. **Separation of Concerns**
- Window: SDL window creation
- Renderer: OpenGL rendering
- InputManager: Input state tracking
- Application: Coordination only

---

## Performance Targets

```
┌─────────────────────────────────────┐
│  Frame Budget: 16.67ms (60fps)      │
├─────────────────────────────────────┤
│  Input:        < 0.1ms              │
│  Update:       < 10ms (future)      │
│  Render:       < 6ms                │
└─────────────────────────────────────┘
```

---

## Future Extensions

### Next Steps:
1. **Math Library** (Vector2, Matrix, Transform)
2. **Sprite System** (Texture loading, batching)
3. **ECS** (Entity-Component-System)
4. **Physics** (Collision detection, spatial partitioning)
5. **Audio System**

### Architecture Evolution:
```
Current:
Application → Window, Input, Renderer

After ECS:
Application → Scene → EntityManager
                    → SystemManager
                      ├─ RenderSystem
                      ├─ PhysicsSystem
                      └─ InputSystem
```

---

## Testing the Input System

Add this to `Application::run()` after `processEvents()`:

```cpp
// Test ESC to quit
if (m_inputManager->isKeyPressed(SDL_SCANCODE_ESCAPE)) {
    m_running = false;
    SDL_Log("ESC pressed - exiting!");
}

// Test SPACE key
if (m_inputManager->isKeyJustPressed(SDL_SCANCODE_SPACE)) {
    SDL_Log("SPACE just pressed at (%d, %d)",
            m_inputManager->getMouseX(),
            m_inputManager->getMouseY());
}
```

---

**Status**: Input system fully integrated ✅
**Next**: Math library and sprite rendering
