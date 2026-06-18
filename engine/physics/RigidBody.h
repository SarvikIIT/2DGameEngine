#pragma once
#include "engine/math/Vector2.h"
#include "engine/math/AABB.h"

namespace Engine {
namespace Physics {

/**
 * @brief A 2D physics body: an axis-aligned box with motion and material properties.
 *
 * Plain data (a struct) — the simulation logic lives in PhysicsWorld. A body is
 * either dynamic (moves, responds to collisions) or static (immovable: walls,
 * ground). Its collision shape is an AABB centered on @ref position.
 */
struct RigidBody {
    Math::Vector2 position;    // center of the body, world space
    Math::Vector2 velocity;    // units per second
    Math::Vector2 halfSize;    // half-extents: position ± halfSize spans the box
    float mass;                // larger mass = pushed less in a collision
    float restitution;         // bounciness: 0 = no bounce (thud), 1 = perfect bounce
    bool  isStatic;            // true = never moves (skips integration & positional push)

    RigidBody()
        : position(0.0f, 0.0f),
          velocity(0.0f, 0.0f),
          halfSize(0.5f, 0.5f),
          mass(1.0f),
          restitution(0.0f),
          isStatic(false) {}

    /**
     * @brief This body's collision box, centered on its current position.
     */
    Math::AABB getAABB() const {
        return Math::AABB(position - halfSize, position + halfSize);
    }
};

} // namespace Physics
} // namespace Engine
