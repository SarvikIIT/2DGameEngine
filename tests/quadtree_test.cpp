// Standalone smoke test for the Quadtree — no SDL/OpenGL, just the tree.
// Build target: QuadtreeTest
#include "engine/physics/Quadtree.h"
#include "engine/math/AABB.h"
#include "engine/math/Vector2.h"
#include <iostream>

using namespace Engine::Math;
using namespace Engine::Physics;

// helper: make an entity as a small box centered at (x, y)
static QuadtreeEntity makeEntity(float x, float y, float size = 2.0f) {
    AABB box = AABB::fromCenterSize(Vector2(x, y), Vector2(size, size));
    return QuadtreeEntity(box, nullptr);
}

int main() {
    // World spans [0,0] to [100,100]. capacity = 4, maxDepth = 6.
    AABB world(Vector2(0, 0), Vector2(100, 100));
    Quadtree tree(world, 4, 6);

    // Insert a 10x10 grid → 100 entities (forces lots of subdivision)
    int inserted = 0;
    for (int x = 5; x < 100; x += 10)
        for (int y = 5; y < 100; y += 10)
            if (tree.insert(makeEntity((float)x, (float)y))) inserted++;

    std::cout << "Inserted:        " << inserted        << " (expected 100)\n";
    std::cout << "tree.size():     " << tree.size()     << " (expected 100)\n";

    // Out-of-bounds insert must be rejected
    bool oob = tree.insert(makeEntity(500.0f, 500.0f));
    std::cout << "OOB insert:      " << (oob ? "true" : "false") << " (expected false)\n";

    // Query a sub-region [0,0]-[25,25]
    std::vector<QuadtreeEntity*> hits;
    size_t n = tree.query(AABB(Vector2(0, 0), Vector2(25, 25)), hits);
    std::cout << "Query [0-25]:    " << n << " entities\n";

    // Full-world query MUST return every entity (this catches the
    // 'empty internal node skips children' class of bugs)
    std::vector<QuadtreeEntity*> all;
    size_t total = tree.query(world, all);
    std::cout << "Full query:      " << total << " (expected " << tree.size() << ")\n";

    bool pass = (inserted == 100) && (tree.size() == 100) && (!oob) && (total == tree.size());
    std::cout << "\n" << (pass ? "=== PASS ===" : "=== FAIL ===") << "\n";
    return pass ? 0 : 1;
}
