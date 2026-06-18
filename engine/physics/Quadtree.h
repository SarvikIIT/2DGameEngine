#pragma once
#include "../math/AABB.h"
#include "../math/Vector2.h"
#include <vector>
#include <memory>

namespace Engine {
namespace Physics {

/**
 * @brief Entity interface for objects stored in the quadtree
 *
 * Any object that needs spatial partitioning must provide a bounding box.
 */
struct QuadtreeEntity {
    Math::AABB bounds;      // Bounding box for spatial queries
    void* userData;         // User-defined pointer to actual entity

    QuadtreeEntity() : userData(nullptr) {}
    QuadtreeEntity(const Math::AABB& aabb, void* data = nullptr)
        : bounds(aabb), userData(data) {}
};

/**
 * @brief High-performance Quadtree for spatial partitioning in 2D
 *
 * A 2D generalization of BST/segment trees:
 * - Each node partitions 2D space into 4 quadrants (NW, NE, SW, SE)
 * - Recursively subdivides when node capacity is exceeded
 * - Optimized for broad-phase collision detection and range queries
 *
 * Performance characteristics:
 * - Construction: O(n log n) average case
 * - Insertion: O(log n) average case
 * - Query: O(log n + k) where k is number of results
 * - Range query: O(log n + k) for rectangular regions
 *
 * Memory layout optimized for cache efficiency with custom allocators.
 *
 * Usage:
 * @code
 *   Quadtree tree(worldBounds, 8, 6);  // capacity=8, maxDepth=6
 *   tree.insert(entity);
 *   std::vector<QuadtreeEntity*> results;
 *   tree.query(queryRegion, results);
 * @endcode
 */
class Quadtree {
public:
    /**
     * @brief Configuration parameters for quadtree construction
     */
    struct Config {
        size_t nodeCapacity;     // Max entities per node before subdivision
        size_t maxDepth;         // Maximum tree depth to prevent over-subdivision

        Config(size_t capacity = 8, size_t depth = 6)
            : nodeCapacity(capacity), maxDepth(depth) {}
    };

    // ===== CONSTRUCTORS =====

    /**
     * @brief Construct quadtree with given boundary and configuration
     * @param boundary World space boundary for the entire tree
     * @param nodeCapacity Maximum entities per node before subdivision (default: 8)
     * @param maxDepth Maximum tree depth (default: 6)
     */
    Quadtree(const Math::AABB& boundary, size_t nodeCapacity = 8, size_t maxDepth = 6);

    /**
     * @brief Construct quadtree with configuration object
     * @param boundary World space boundary
     * @param config Configuration parameters
     */
    Quadtree(const Math::AABB& boundary, const Config& config);

    /**
     * @brief Destructor - cleans up tree structure
     */
    ~Quadtree();

    // Prevent copying (can be expensive for large trees)
    Quadtree(const Quadtree&) = delete;
    Quadtree& operator=(const Quadtree&) = delete;

    // Allow moving
    Quadtree(Quadtree&& other) noexcept;
    Quadtree& operator=(Quadtree&& other) noexcept;

    // ===== CORE OPERATIONS =====

    /**
     * @brief Insert an entity into the quadtree
     * @param entity Entity with bounding box to insert
     * @return true if successfully inserted, false if outside boundary
     *
     * Complexity: O(log n) average case
     */
    bool insert(const QuadtreeEntity& entity);

    /**
     * @brief Remove an entity from the quadtree
     * @param entity Entity to remove (matches by pointer equality)
     * @return true if entity was found and removed
     *
     * Complexity: O(log n) average case
     */
    bool remove(const QuadtreeEntity& entity);

    /**
     * @brief Clear all entities from the tree
     *
     * Complexity: O(1) - reuses allocated memory
     */
    void clear();

    /**
     * @brief Rebuild the entire tree structure
     *
     * Useful after many removals to rebalance the tree.
     * Complexity: O(n log n)
     */
    void rebuild();

    // ===== QUERY OPERATIONS =====

    /**
     * @brief Query all entities within a rectangular region
     * @param range AABB region to query
     * @param results Output vector of entities in range
     * @return Number of entities found
     *
     * Complexity: O(log n + k) where k is number of results
     */
    size_t query(const Math::AABB& range, std::vector<QuadtreeEntity*>& results) const;

    /**
     * @brief Query all entities within a circular region
     * @param center Center of query circle
     * @param radius Radius of query circle
     * @param results Output vector of entities in range
     * @return Number of entities found
     *
     * Note: Uses AABB approximation for broad phase, then refines
     */
    size_t queryCircle(const Math::Vector2& center, float radius,
                       std::vector<QuadtreeEntity*>& results) const;

    /**
     * @brief Query all entities that could potentially collide with a given AABB
     * @param bounds Bounding box to test
     * @param results Output vector of potential collision candidates
     * @return Number of candidates found
     *
     * Used for broad-phase collision detection
     */
    size_t queryCollisionCandidates(const Math::AABB& bounds,
                                    std::vector<QuadtreeEntity*>& results) const;

    /**
     * @brief Find nearest entity to a point
     * @param point Query point
     * @param maxDistance Maximum search distance (default: infinite)
     * @return Pointer to nearest entity, or nullptr if none found
     */
    QuadtreeEntity* queryNearest(const Math::Vector2& point,
                                 float maxDistance = INFINITY) const;

    /**
     * @brief Find K nearest entities to a point
     * @param point Query point
     * @param k Number of nearest neighbors to find
     * @param results Output vector of nearest entities (sorted by distance)
     * @param maxDistance Maximum search distance (default: infinite)
     * @return Number of entities found
     */
    size_t queryKNearest(const Math::Vector2& point, size_t k,
                         std::vector<QuadtreeEntity*>& results,
                         float maxDistance = INFINITY) const;

    // ===== FRUSTUM CULLING =====

    /**
     * @brief Query entities visible in camera frustum (viewport)
     * @param viewport Camera viewport bounds
     * @param results Output vector of visible entities
     * @return Number of visible entities
     *
     * Optimized for rendering pipeline - used by SpriteRenderer
     */
    size_t queryFrustum(const Math::AABB& viewport,
                        std::vector<QuadtreeEntity*>& results) const;

    // ===== STATISTICS & DEBUGGING =====

    /**
     * @brief Get total number of entities in the tree
     * @return Entity count
     */
    size_t size() const;

    /**
     * @brief Check if tree is empty
     * @return true if no entities stored
     */
    bool empty() const;

    /**
     * @brief Get current depth of the tree
     * @return Actual maximum depth
     */
    size_t getDepth() const;

    /**
     * @brief Get total number of nodes in the tree
     * @return Node count (including subdivided nodes)
     */
    size_t getNodeCount() const;

    /**
     * @brief Get memory usage in bytes
     * @return Total memory used by tree structure
     */
    size_t getMemoryUsage() const;

    /**
     * @brief Get the root boundary of the tree
     * @return World space AABB
     */
    const Math::AABB& getBoundary() const;

    /**
     * @brief Get tree configuration
     * @return Configuration parameters
     */
    const Config& getConfig() const;

    /**
     * @brief Collect all entities in the tree (for debugging)
     * @param results Output vector
     * @return Total entity count
     */
    size_t getAllEntities(std::vector<QuadtreeEntity*>& results) const;

    /**
     * @brief Validate tree structure integrity
     * @return true if tree is valid and consistent
     *
     * For debugging - checks invariants:
     * - All entities are within boundaries
     * - No duplicate entities
     * - Node capacity constraints respected
     */
    bool validate() const;

    // ===== PERFORMANCE METRICS =====

    /**
     * @brief Performance statistics for profiling
     */
    struct Statistics {
        size_t totalEntities;       // Total entities stored
        size_t totalNodes;          // Total nodes allocated
        size_t leafNodes;           // Number of leaf nodes
        size_t subdivisions;        // Number of subdivided nodes
        size_t maxDepthReached;     // Deepest level reached
        size_t entitiesPerNode;     // Average entities per leaf node
        size_t memoryBytes;         // Total memory usage

        Statistics() : totalEntities(0), totalNodes(0), leafNodes(0),
                      subdivisions(0), maxDepthReached(0),
                      entitiesPerNode(0), memoryBytes(0) {}
    };

    /**
     * @brief Get detailed performance statistics
     * @return Statistics structure with tree metrics
     */
    Statistics getStatistics() const;

    /**
     * @brief Reset query performance counters
     *
     * For benchmarking - clears internal query counters
     */
    void resetPerformanceCounters();

private:
    // Forward declaration of internal node structure
    struct QuadtreeNode;

    // Private members
    Math::AABB m_boundary;          // Root boundary
    Config m_config;                // Tree configuration
    std::unique_ptr<QuadtreeNode> m_root;  // Root node
    size_t m_entityCount;           // Total entities in tree

    // Private helper methods (to be implemented in .cpp)

    /**
     * @brief Recursively insert entity into tree
     */
    bool insertRecursive(QuadtreeNode* node, const QuadtreeEntity& entity, size_t depth);

    /**
     * @brief Recursively remove entity from tree
     */
    bool removeRecursive(QuadtreeNode* node, const QuadtreeEntity& entity);

    /**
     * @brief Recursively query entities in range
     */
    void queryRecursive(const QuadtreeNode* node, const Math::AABB& range,
                        std::vector<QuadtreeEntity*>& results) const;

    /**
     * @brief Recursively find nearest entity
     */
    void queryNearestRecursive(const QuadtreeNode* node, const Math::Vector2& point,
                               QuadtreeEntity*& nearest, float& nearestDistSq,
                               float maxDistSq) const;

    /**
     * @brief Subdivide a node into 4 quadrants
     */
    void subdivide(QuadtreeNode* node);

    /**
     * @brief Try to collapse a node (merge children)
     */
    bool tryCollapse(QuadtreeNode* node);

    /**
     * @brief Calculate node depth recursively
     */
    size_t calculateDepth(const QuadtreeNode* node) const;

    /**
     * @brief Count nodes recursively
     */
    size_t countNodes(const QuadtreeNode* node) const;

    /**
     * @brief Calculate memory usage recursively
     */
    size_t calculateMemoryUsage(const QuadtreeNode* node) const;

    /**
     * @brief Collect statistics recursively
     */
    void collectStatistics(const QuadtreeNode* node, Statistics& stats, size_t depth) const;

    /**
     * @brief Validate node recursively
     */
    bool validateNode(const QuadtreeNode* node, size_t depth) const;
};

} // namespace Physics
} // namespace Engine
