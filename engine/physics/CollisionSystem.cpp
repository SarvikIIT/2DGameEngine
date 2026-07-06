#include "CollisionSystem.h"
#include <functional>

namespace Engine {
namespace Physics {
    CollisionSystem::CollisionSystem(const Math::AABB& worldBounds, size_t nodeCapacity, size_t maxDepth) :
        m_tree(worldBounds, nodeCapacity, maxDepth) {}
    void CollisionSystem::update(const std::vector<QuadtreeEntity>& colliders) {
        m_tree.clear();
        for (const auto& entity : colliders) {
            m_tree.insert(entity);
        }
        m_collisions.clear();
        std::vector<QuadtreeEntity*> candidates;
        for(const auto& entity : colliders){
            candidates.clear();
            m_tree.query(entity.bounds, candidates);
            for(const auto& candidate : candidates){
                if(candidate->userData == entity.userData) continue; // skip self
                if(candidate->bounds.intersects(entity.bounds)){
                    // To avoid duplicates, only add pairs where candidate < entity (by pointer)
                    if(candidate->userData < entity.userData){
                        m_collisions.push_back({candidate->userData, entity.userData});
                    }
                }
            }
        }
    }
} // namespace Physics
} // namespace Engine