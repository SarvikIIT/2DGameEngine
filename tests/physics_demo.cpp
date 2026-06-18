// ASCII physics demo — watch balls fall onto a floor and pile up.
// Uses PhysicsWorld (gravity + collision detection + separation). No graphics.
// Build target: PhysicsDemo
#include "engine/physics/PhysicsWorld.h"
#include "engine/physics/RigidBody.h"
#include "engine/math/AABB.h"
#include "engine/math/Vector2.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace Engine::Math;
using namespace Engine::Physics;

static const int   W = 50;     // grid width  (world x: 0..W)
static const int   H = 24;     // grid height (world y: 0..H)
static const float DT = 1.0f / 30.0f;
static const int   FRAMES = 200;

// Make a dynamic ball at (x, y) with the given half-size and bounciness.
static RigidBody makeBall(float x, float y, float r, float restitution = 0.6f) {
    RigidBody b;
    b.position = Vector2(x, y);
    b.halfSize = Vector2(r, r);
    b.velocity = Vector2(0.0f, 0.0f);
    b.mass = 1.0f;
    b.restitution = restitution;
    b.isStatic = false;
    return b;
}

// Draw the world as an ASCII grid: '#' = static (floor), 'O' = dynamic (ball).
static void render(const PhysicsWorld& world, int frame) {
    std::string screen;
    screen.reserve((W + 1) * H);
    for (int row = 0; row < H; ++row) {
        for (int col = 0; col < W; ++col) {
            float wx = col + 0.5f;
            float wy = (H - 1 - row) + 0.5f;        // flip y so "up" is up
            char c = ' ';
            for (size_t i = 0; i < world.getBodyCount(); ++i) {
                const RigidBody& b = world.getBody(i);
                if (b.getAABB().contains(Vector2(wx, wy))) {
                    c = b.isStatic ? '#' : 'O';
                    break;
                }
            }
            screen += c;
        }
        screen += '\n';
    }
    std::cout << "\x1b[H";                            // move cursor home (overwrite)
    std::cout << "Frame " << frame << " / " << FRAMES << "    \n";
    std::cout << screen << std::flush;
}

int main() {
    AABB worldBounds(Vector2(0, 0), Vector2((float)W, (float)H));
    PhysicsWorld world(worldBounds);
    world.setGravity(Vector2(0.0f, -30.0f));

    // A floor across the bottom (static, immovable).
    RigidBody floor;
    floor.position = Vector2(W / 2.0f, 1.5f);
    floor.halfSize = Vector2(W / 2.0f, 1.5f);
    floor.isStatic = true;
    world.addBody(floor);

    // Drop several balls from different heights/columns. Two share a column so
    // they collide with EACH OTHER (dynamic-vs-dynamic) and stack.
    world.addBody(makeBall(10.0f, 22.0f, 1.5f, 0.8f));   // very bouncy
    world.addBody(makeBall(25.0f, 20.0f, 2.0f, 0.5f));
    world.addBody(makeBall(40.0f, 23.0f, 1.5f, 0.7f));
    world.addBody(makeBall(25.0f, 14.0f, 2.0f, 0.5f));   // above the 2nd ball -> stacks
    world.addBody(makeBall(33.0f, 18.0f, 1.0f, 0.9f));   // super bouncy

    std::cout << "\x1b[2J";                           // clear screen once
    for (int frame = 0; frame < FRAMES; ++frame) {
        world.step(DT);
        render(world, frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    std::cout << "\nDone.\n";
    return 0;
}
