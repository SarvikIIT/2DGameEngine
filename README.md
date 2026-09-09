<h1 align="center">Axiom</h1>

<p align="center">
  <em>A 2D game engine written from the maths up.</em><br>
  <sub>C++17 · SDL3 · OpenGL · CMake</sub>
</p>

---

Everything above the platform layer is hand-written: vectors and matrices,
the intersection tests, the spatial index, the sprite batcher. SDL3 supplies a
window and an input event queue, OpenGL draws triangles, and nothing else is
borrowed. The point was to understand the parts of an engine that are usually
opaque, so each one is implemented rather than pulled in.

Three pieces carry most of the work.

- **Broad phase.** Collision detection is quadratic if every body is tested
  against every other. A quadtree over the world bounds keeps each body's
  candidate set to the bodies sharing its region, so the pair count falls from
  every pair to the pairs that are plausibly touching.
- **Sprite batching.** Drawing a thousand sprites with a thousand draw calls
  spends its time in driver overhead rather than on the GPU. Vertices for a
  frame are accumulated into one dynamic vertex buffer and issued as a single
  `glDrawArrays`, which for 1000 sprites holds 166 to 168 FPS.
- **The maths layer.** `Vector2/3/4`, `Matrix2x2/3x3/4x4`, AABB and circle
  primitives, and Liang-Barsky for line against box. Written out because the
  clipping and the transform stack are the parts worth knowing.

Physics runs the standard three stages per step: integrate under gravity,
detect overlapping pairs through the broad and narrow phases, then resolve by
separating the bodies and reflecting their velocities. Bodies are plain structs
held in one contiguous vector and referred to by handle, so the simulation loop
walks memory in order and the caller never holds a pointer that a resize can
invalidate.

The broad-phase claim is not taken on faith. `Benchmark` counts the pairs the
quadtree actually tests against the pairs brute force would, and times a full
step against a 60 fps budget. It is headless and seeded, so the numbers
reproduce.

## Layout

| | |
| --- | --- |
| `engine/math/` | Vectors, matrices, AABB, circle, Liang-Barsky clipping. |
| `engine/physics/` | Quadtree, broad and narrow phase, rigid bodies, the step loop. |
| `engine/renderer/` | Sprite batcher, shaders, textures, 2D camera with culling. |
| `engine/core/` | Window, application loop, input. |
| `main.cpp` | The windowed demo. |
| `tests/` | Quadtree smoke test, headless ASCII physics demo, broad-phase benchmark. |
| `docs/ARCHITECTURE.md` | How the pieces fit together. |

## Build

Dependencies come from vcpkg. It is not vendored here, and `CMakeLists.txt`
points its toolchain file at `vcpkg/` in the project root, so clone and
bootstrap it there first or configure fails on the missing toolchain:

```bash
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh        # Windows: .\vcpkg\bootstrap-vcpkg.bat

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

`vcpkg.json` is a manifest, so SDL3, glad and stb are fetched during configure
rather than installed by hand.

`GameEngine` needs SDL3 and OpenGL, and its stb include path is currently
pinned to the `x64-windows` triplet — elsewhere that line needs adjusting. The
three test targets need neither SDL nor OpenGL and build anywhere.

## Run

```bash
./build/GameEngine        # windowed demo: WASD pans the camera, Q/E zooms
./build/QuadtreeTest      # spatial index smoke test
./build/PhysicsDemo       # headless physics, rendered as ASCII
./build/Benchmark         # broad-phase pair counts and step timing
```
