#pragma once
#include "Quadtree.h"   // brings in QuadtreeEntity, Math::AABB, std::vector
#include <vector>

namespace Engine {
namespace Physics {

/**
 * @brief A pair of entities whose bounding boxes overlap this frame.
 *
 * Identified by userData (the caller's real object), so the consumer can map
 * a collision back to its own game objects without the physics layer knowing
 * what they are.
 */
struct CollisionPair {
    void* a;   // userData of the first entity
    void* b;   // userData of the second entity
};

/**
 * @brief Broad-phase collision detection over a set of moving entities.
 *
 * Wraps a Quadtree and, each frame, answers "which entities overlap?".
 *
 * Usage (rebuild-every-frame model):
 * @code
 *   CollisionSystem physics(worldBounds);
 *   physics.update(colliders);                 // colliders = this frame's entities
 *   for (const auto& pair : physics.getCollisions())
 *       resolve(pair.a, pair.b);
 * @endcode
 *
 * Each overlapping pair is reported exactly once (no (A,B) and (B,A) duplicates).
 */
class CollisionSystem {
public:
    /**
     * @brief Construct over a fixed world region.
     * @param worldBounds   Outer boundary of the simulated space (the quadtree root).
     * @param nodeCapacity  Entities per quadtree node before it subdivides.
     * @param maxDepth      Maximum quadtree depth.
     */
    CollisionSystem(const Math::AABB& worldBounds,
                    size_t nodeCapacity = 8,
                    size_t maxDepth = 6);

    /**
     * @brief Recompute all overlapping pairs for the current frame.
     *
     * Rebuilds the spatial index from @p colliders, then runs broad-phase
     * (quadtree candidates) + narrow-phase (exact AABB test). Results are
     * available via getCollisions() until the next update().
     *
     * @param colliders  Every entity to test this frame (bounds + userData).
     */
    void update(const std::vector<QuadtreeEntity>& colliders);

    /**
     * @brief The overlapping pairs found by the most recent update().
     * @return Reference to the internal results (valid until the next update()).
     */
    const std::vector<CollisionPair>& getCollisions() const { return m_collisions; }

    /**
     * @brief Number of colliding pairs from the most recent update().
     */
    size_t getCollisionCount() const { return m_collisions.size(); }

private:
    Quadtree                   m_tree;        // spatial index, rebuilt each update()
    std::vector<CollisionPair> m_collisions;  // results of the most recent update()
};

} // namespace Physics
} // namespace Engine
