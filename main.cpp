#include "engine/core/Application.h"
#include <iostream>

int main(int argc, char* args[]) {
    Engine::Core::Application app("2D Game Engine", 800, 600);
    if (!app.init()) {
        std::cerr << "Failed to initialize application!\n";
        return 1;
    }
    app.run();
    app.shutdown();
    return 0;
}
