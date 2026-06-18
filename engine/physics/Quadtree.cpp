#include "Quadtree.h"
#include <algorithm>

namespace Engine {
namespace Physics {
    struct Quadtree::QuadtreeNode {
        Math::AABB boundary;
        std::vector<QuadtreeEntity> entities;
        std::unique_ptr<QuadtreeNode> children[4];
        QuadtreeNode(const Math::AABB& boundary) : boundary(boundary) {};
        bool isLeaf() const {
            return children[0] == nullptr;
        };
    };

    // Which child quadrant fully contains the entity? Returns 0..3, or -1 if it
    // straddles the center (and therefore must stay at the parent node).
    // Shared by insert and remove so the descent logic stays identical.
    // Index convention matches subdivide(): 0=SW, 1=SE, 2=NW, 3=NE.
    static int quadrantFor(const Math::Vector2& center, const QuadtreeEntity& e) {
        const Math::AABB& b = e.bounds;
        bool straddlesX = (b.min.x < center.x) && (b.max.x > center.x);
        bool straddlesY = (b.min.y < center.y) && (b.max.y > center.y);
        if (straddlesX || straddlesY) return -1;
        bool right = b.min.x >= center.x;
        bool top   = b.min.y >= center.y;
        return (top ? 2 : 0) + (right ? 1 : 0);
    }

    Quadtree::Quadtree(const Math::AABB& boundary, size_t nodeCapacity, size_t maxDepth) :
        m_boundary(boundary) , m_config(nodeCapacity, maxDepth), m_root(std::make_unique<QuadtreeNode>(boundary)), m_entityCount(0){}

    Quadtree:: ~Quadtree() = default;

    Quadtree::Quadtree(const Math::AABB& boundary, const Config& config) :
        m_boundary(boundary), m_config(config), m_root(std::make_unique<QuadtreeNode>(boundary)), m_entityCount(0) {}

    void Quadtree::subdivide(QuadtreeNode* node) {
        const Math::Vector2 &min = node->boundary.min;
        const Math::Vector2 &max = node->boundary.max;
        Math::Vector2 center = node->boundary.getCenter();
        node->children[0] = std::make_unique<QuadtreeNode>(Math::AABB(min, center));
        node->children[1] = std::make_unique<QuadtreeNode>(Math::AABB(Math::Vector2(center.x, min.y), Math::Vector2(max.x, center.y)));
        node->children[2] = std::make_unique<QuadtreeNode>(Math::AABB(Math::Vector2(min.x, center.y), Math::Vector2(center.x, max.y)));
        node->children[3] = std::make_unique<QuadtreeNode>(Math::AABB(center, max));
    }

    bool Quadtree::insert(const QuadtreeEntity& entity) {
        if(!m_boundary.intersects(entity.bounds)) return false;
        insertRecursive(m_root.get(), entity, 0);
        m_entityCount++;
        return true;
    }

    bool Quadtree::insertRecursive(QuadtreeNode *node, const QuadtreeEntity &entity, size_t depth){
        // Case 1 : Internal node -> go deeper
        if(!node->isLeaf()){
            int quadrant = quadrantFor(node->boundary.getCenter(), entity);
            if(quadrant != -1){
                return insertRecursive(node->children[quadrant].get(), entity, depth + 1);
            }
            node->entities.push_back(entity);
            return true;
        }
        // Case 2 : Leaf node in which we can insert the entity
        if(node->entities.size() < m_config.nodeCapacity || depth >= m_config.maxDepth){
            node->entities.push_back(entity);
            return true;
        }
        // Case 3 : Leaf node has to be subdivided
        subdivide(node);
        std::vector<QuadtreeEntity> oldEntities = std::move(node->entities);
        for(const auto& e: oldEntities){
            int quadrant = quadrantFor(node->boundary.getCenter(), e);
            if(quadrant != -1){
                insertRecursive(node->children[quadrant].get(), e, depth + 1);
            } else {
                node->entities.push_back(e);
            }
        }
        int quadrant = quadrantFor(node->boundary.getCenter(), entity);
        if(quadrant != -1){
            return insertRecursive(node->children[quadrant].get(), entity, depth + 1);
        } else {
            node->entities.push_back(entity);
            return true;
        }
    }

    size_t Quadtree::query(const Math::AABB& range, std::vector<QuadtreeEntity*> &results) const {
        results.clear();
        queryRecursive(m_root.get(), range, results);
        return results.size();
    }

    void Quadtree::queryRecursive(const QuadtreeNode* node, const Math::AABB& range, std::vector<QuadtreeEntity*>& results) const {
        if(!node->boundary.intersects(range)) return;
        for(const auto& entity : node->entities){
            if(range.intersects(entity.bounds)){
                results.push_back(const_cast<QuadtreeEntity*>(&entity));
            }
        }
        if(!node->isLeaf()){
            for(const auto& child: node->children){
                queryRecursive(child.get(), range, results);
            }
        }
    }

    size_t Quadtree::size() const {return m_entityCount;}

    bool Quadtree::empty() const {return m_entityCount == 0;}

    const Math::AABB& Quadtree::getBoundary() const {return m_boundary;}

    const Quadtree::Config& Quadtree::getConfig() const {return m_config;}

    void Quadtree::clear() {
        m_root = std::make_unique<QuadtreeNode>(m_boundary);
        m_entityCount = 0;
    }

    size_t Quadtree::queryCollisionCandidates(const Math::AABB& bounds, std::vector<QuadtreeEntity*>& results) const {
        return query(bounds, results);
    }


} // namespace Physics
} // namespace Engine