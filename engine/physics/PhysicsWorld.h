#pragma once
#include "RigidBody.h"
#include "CollisionSystem.h"
#include <vector>

namespace Engine {
namespace Physics {

/**
 * @brief Owns a set of rigid bodies and advances the 2D simulation.
 *
 * Each step() runs the classic three-stage physics loop:
 *   1. INTEGRATE — apply gravity and move dynamic bodies by their velocity
 *   2. DETECT    — find overlapping pairs (broad + narrow phase, via CollisionSystem)
 *   3. RESOLVE   — separate overlapping bodies and adjust their velocities (bounce)
 *
 * Usage (the caller drives a fixed timestep):
 * @code
 *   PhysicsWorld world(worldBounds);
 *   size_t ball = world.addBody(makeBall());
 *   world.setGravity({0.0f, -9.8f});
 *   // each frame:
 *   world.step(1.0f / 60.0f);
 *   Vector2 p = world.getBody(ball).position;   // read back for rendering
 * @endcode
 */
class PhysicsWorld {
public:
    /**
     * @brief Construct over a fixed world region (the spatial index boundary).
     * @param worldBounds  Outer boundary of the simulated space.
     */
    explicit PhysicsWorld(const Math::AABB& worldBounds);

    /**
     * @brief Add a body to the simulation.
     * @return A stable index (handle) used to read/modify the body later.
     *
     * Note: add all bodies before stepping; indices stay valid for the world's
     * lifetime, but raw pointers into the body storage may not.
     */
    size_t addBody(const RigidBody& body);

    /** @brief Access a body by its handle (to set velocity, read position, ...). */
    RigidBody&       getBody(size_t index);
    const RigidBody& getBody(size_t index) const;

    /** @brief Number of bodies in the world. */
    size_t getBodyCount() const { return m_bodies.size(); }

    /**
     * @brief Advance the simulation by one fixed timestep.
     * @param dt  Fixed step in seconds (e.g. 1/60). Use a constant value for
     *            stability — variable dt makes the simulation jittery.
     */
    void step(float dt);

    /** @brief Constant acceleration applied to dynamic bodies each step (e.g. gravity). */
    void setGravity(const Math::Vector2& gravity) { m_gravity = gravity; }
    const Math::Vector2& getGravity() const { return m_gravity; }

private:
    // Stage 2: rebuild the spatial index from the bodies and find overlapping pairs.
    void detectCollisions();

    // Stage 3: push overlapping bodies apart and adjust their velocities (bounce).
    void resolveCollisions();

    std::vector<RigidBody> m_bodies;          // all simulated bodies
    CollisionSystem        m_collisionSystem; // broad+narrow phase (rebuilt each step)
    Math::Vector2          m_gravity;         // applied during integration
};

} // namespace Physics
} // namespace Engine
