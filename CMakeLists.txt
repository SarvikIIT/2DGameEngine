# Minimum CMake version required
cmake_minimum_required(VERSION 3.20)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
# Project name and version
project(2DGameEngine VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find packages
find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# Engine source files
set(ENGINE_SOURCES
    engine/core/Window.cpp
    engine/core/Application.cpp
    engine/core/InputManager.cpp
    engine/renderer/Renderer.cpp
    engine/renderer/Texture.cpp
    engine/renderer/Shader.cpp
    engine/renderer/Sprite.cpp
    engine/renderer/SpriteRenderer.cpp
    engine/renderer/Camera2D.cpp
    engine/math/Vector2.cpp
    engine/math/Vector3.cpp
    engine/math/Vector4.cpp
    engine/math/Matrix2x2.cpp
    engine/math/Matrix3x3.cpp
    engine/math/Matrix4x4.cpp
    engine/math/AABB.cpp
    engine/math/Circle.cpp
    engine/math/Collision.cpp
    engine/physics/Quadtree.cpp
    engine/physics/CollisionSystem.cpp
    engine/physics/PhysicsWorld.cpp
)

# Create executable
add_executable(GameEngine
    main.cpp
    ${ENGINE_SOURCES}
)

# Include directories (so #include "engine/core/Window.h" works)
target_include_directories(GameEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/packages/stb_x64-windows/include
)

# Link libraries
target_link_libraries(GameEngine PRIVATE
    SDL3::SDL3
    glad::glad
    OpenGL::GL
)

# ---- Quadtree smoke test (standalone, no SDL/OpenGL needed) ----
add_executable(QuadtreeTest
    tests/quadtree_test.cpp
    engine/math/Vector2.cpp
    engine/math/AABB.cpp
    engine/physics/Quadtree.cpp
)
target_include_directories(QuadtreeTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ---- Physics ASCII demo (standalone, no SDL/OpenGL needed) ----
add_executable(PhysicsDemo
    tests/physics_demo.cpp
    engine/math/Vector2.cpp
    engine/math/AABB.cpp
    engine/physics/Quadtree.cpp
    engine/physics/CollisionSystem.cpp
    engine/physics/PhysicsWorld.cpp
)
target_include_directories(PhysicsDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# ---- Headless benchmark: broad-phase pair counts and step timing ----
add_executable(Benchmark
    tests/benchmark.cpp
    engine/math/Vector2.cpp
    engine/math/AABB.cpp
    engine/physics/Quadtree.cpp
    engine/physics/CollisionSystem.cpp
    engine/physics/PhysicsWorld.cpp
)
target_include_directories(Benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
