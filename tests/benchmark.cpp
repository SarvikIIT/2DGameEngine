// Measures the two claims the engine makes about its broad phase:
//   1. the quadtree removes most of the pair tests brute force would do;
//   2. a full physics step at N bodies fits inside a 60 fps frame budget.
//
// Headless: no SDL, no OpenGL. Deterministic: fixed seed, so the numbers
// reproduce.

#include <chrono>
#include <cstdio>
#include <random>
#include <vector>

#include "engine/physics/CollisionSystem.h"
#include "engine/physics/PhysicsWorld.h"

using namespace Engine;
using Clock = std::chrono::steady_clock;

namespace {

constexpr float kWorld = 1000.0f;
constexpr float kHalf = 2.0f;  // body half-extent

Math::AABB world_bounds() {
    return Math::AABB(Math::Vector2(0.0f, 0.0f), Math::Vector2(kWorld, kWorld));
}

std::vector<Math::Vector2> scatter(int n, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> u(kHalf, kWorld - kHalf);
    std::vector<Math::Vector2> pts;
    pts.reserve(n);
    for (int i = 0; i < n; ++i) pts.push_back({u(rng), u(rng)});
    return pts;
}

// Pair tests brute force performs: every unordered pair, n(n-1)/2.
long long brute_force_tests(int n) {
    return static_cast<long long>(n) * (n - 1) / 2;
}

// Pair tests the quadtree actually performs, counted by instrumenting the same
// query the broad phase runs: for each body, how many candidates come back.
long long quadtree_tests(const std::vector<Math::Vector2>& pts) {
    Physics::Quadtree tree(world_bounds(), 8, 6);
    std::vector<Physics::QuadtreeEntity> ents;
    ents.reserve(pts.size());
    for (std::size_t i = 0; i < pts.size(); ++i) {
        Math::AABB box(pts[i] - Math::Vector2(kHalf, kHalf), pts[i] + Math::Vector2(kHalf, kHalf));
        ents.push_back({box, reinterpret_cast<void*>(i + 1)});
    }
    for (const auto& e : ents) tree.insert(e);

    long long tests = 0;
    for (const auto& e : ents) {
        std::vector<Physics::QuadtreeEntity> found;
        tree.query(e.bounds, found);
        tests += static_cast<long long>(found.size()) - 1;  // exclude self
    }
    return tests / 2;  // each pair surfaces from both sides
}

void broad_phase_table() {
    std::printf("\nBroad phase: pair tests performed\n");
    std::printf("%8s %14s %12s %10s\n", "bodies", "brute force", "quadtree", "removed");
    for (int n : {1000, 2500, 5000, 10000}) {
        const auto pts = scatter(n, 1234u);
        const long long bf = brute_force_tests(n);
        const long long qt = quadtree_tests(pts);
        std::printf("%8d %14lld %12lld %9.2f%%\n", n, bf, qt,
                    100.0 * (1.0 - static_cast<double>(qt) / static_cast<double>(bf)));
    }
}

void step_time_table() {
    std::printf("\nPhysics step: wall time per step, 60 fps budget is 16.67 ms\n");
    std::printf("%8s %12s %12s %10s\n", "bodies", "ms/step", "fps", "in budget");
    for (int n : {1000, 2500, 5000, 10000}) {
        Physics::PhysicsWorld world(world_bounds());
        const auto pts = scatter(n, 1234u);
        for (const auto& p : pts) {
            Physics::RigidBody b;
            b.position = p;
            b.halfSize = Math::Vector2(kHalf, kHalf);
            b.velocity = Math::Vector2(1.0f, -1.0f);
            world.addBody(b);
        }
        world.setGravity(Math::Vector2(0.0f, -9.8f));

        world.step(1.0f / 60.0f);  // warm up: first step builds the tree
        const int reps = 20;
        const auto t0 = Clock::now();
        for (int i = 0; i < reps; ++i) world.step(1.0f / 60.0f);
        const auto t1 = Clock::now();

        const double ms =
            std::chrono::duration<double, std::milli>(t1 - t0).count() / reps;
        std::printf("%8d %12.3f %12.1f %10s\n", n, ms, 1000.0 / ms,
                    ms <= 16.67 ? "yes" : "no");
    }
}

}  // namespace

int main() {
    std::printf("Axiom benchmark: %.0f x %.0f world, %d-unit bodies, seed 1234\n",
                kWorld, kWorld, static_cast<int>(kHalf * 2));
    broad_phase_table();
    step_time_table();
    std::printf("\n");
    return 0;
}
