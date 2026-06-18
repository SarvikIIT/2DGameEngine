#include "PhysicsWorld.h"
#include <algorithm>
#include <cmath>

namespace Engine {
namespace Physics {
    PhysicsWorld::PhysicsWorld(const Math::AABB& worldBounds)
        : m_bodies(), m_collisionSystem(worldBounds), m_gravity(0.0f, 0.0f) {}
    size_t PhysicsWorld::addBody(const RigidBody& body) {
        m_bodies.push_back(body);
        return m_bodies.size() - 1; // return index of the newly added body
    }
    RigidBody &PhysicsWorld::getBody(size_t index) {
        return m_bodies.at(index); // throws std::out_of_range if index is invalid
    }
    const RigidBody &PhysicsWorld::getBody(size_t index) const {
        return m_bodies.at(index); // throws std::out_of_range if index is invalid
    }
    void PhysicsWorld::step(float dt) {
        for (RigidBody& body : m_bodies) {
            if(!body.isStatic) {
                body.velocity += m_gravity * dt;
                body.position += body.velocity * dt;
            }
        }
        detectCollisions();
        resolveCollisions();
    }
    void PhysicsWorld::detectCollisions() {
        std::vector<QuadtreeEntity> colliders;
        colliders.reserve(m_bodies.size());
        for (RigidBody& body : m_bodies) {
            // userData = pointer to the body, so resolution can recover the RigidBody*
            colliders.emplace_back(body.getAABB(), &body);
        }
        m_collisionSystem.update(colliders);
    }

    void PhysicsWorld::resolveCollisions() {
        for(const auto &pair : m_collisionSystem.getCollisions()){
            RigidBody *body1 = static_cast<RigidBody*>(pair.a);
            RigidBody *body2 = static_cast<RigidBody*>(pair.b);
            if(body1->isStatic && body2->isStatic) continue;
            float overlapX = (body1->halfSize.x + body2->halfSize.x) - std::abs(body1->position.x - body2->position.x);
            float overlapY = (body1->halfSize.y + body2->halfSize.y) - std::abs(body1->position.y - body2->position.y);
            if(overlapX < overlapY) {
                float sign = (body1->position.x < body2->position.x) ? -1.0f : 1.0f;
                if(body1->isStatic) {
                    body2->position.x -= sign * overlapX;
                } else if(body2->isStatic) {
                    body1->position.x += sign * overlapX;
                } else {
                    body1->position.x += sign * (overlapX * 0.5f);
                    body2->position.x -= sign * (overlapX * 0.5f);
                }
                // C2: reflect velocity on the X axis (scaled by restitution)
                if(!body1->isStatic) body1->velocity.x = -body1->velocity.x * body1->restitution;
                if(!body2->isStatic) body2->velocity.x = -body2->velocity.x * body2->restitution;
            } else {
                float sign = (body1->position.y < body2->position.y) ? -1.0f : 1.0f;
                if(body1->isStatic) {
                    body2->position.y -= sign * overlapY;
                } else if(body2->isStatic) {
                    body1->position.y += sign * overlapY;
                } else {
                    body1->position.y += sign * (overlapY * 0.5f);
                    body2->position.y -= sign * (overlapY * 0.5f);
                }
                // C2: reflect velocity on the Y axis (scaled by restitution)
                if(!body1->isStatic) body1->velocity.y = -body1->velocity.y * body1->restitution;
                if(!body2->isStatic) body2->velocity.y = -body2->velocity.y * body2->restitution;
            }
        }
    }

} // namespace Physics
} // namespace Engine