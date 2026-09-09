#pragma once
#include "Vector2.h"

namespace Engine {
namespace Math {

/**
 * @brief Axis-Aligned Bounding Box (AABB) for 2D collision detection
 *
 * A rectangle that cannot rotate, defined by min and max corners.
 * Used for fast collision detection in 2D games.
 */
class AABB {
public:
    Vector2 min;  // Bottom-left corner (smallest x, y)
    Vector2 max;  // Top-right corner (largest x, y)

    // Constructors
    AABB();
    AABB(const Vector2& min, const Vector2& max);
    AABB(float minX, float minY, float maxX, float maxY);

    /**
     * @brief Create AABB from center position and size
     * @param center Center point of the box
     * @param size Width and height (halfSize * 2)
     * @return AABB
     */
    static AABB fromCenterSize(const Vector2& center, const Vector2& size);

    /**
     * @brief Get the center point of the AABB
     * @return Center point
     */
    Vector2 getCenter() const;

    /**
     * @brief Get the size (width, height) of the AABB
     * @return Size vector
     */
    Vector2 getSize() const;

    /**
     * @brief Get the half-extents (half width, half height)
     * @return Half-size vector
     */
    Vector2 getHalfExtents() const;

    /**
     * @brief Get width of the AABB
     * @return Width
     */
    float getWidth() const;

    /**
     * @brief Get height of the AABB
     * @return Height
     */
    float getHeight() const;

    /**
     * @brief Check if a point is inside this AABB
     * @param point Point to test
     * @return true if point is inside
     */
    bool contains(const Vector2& point) const;

    /**
     * @brief Check if this AABB completely contains another AABB
     * @param other Other AABB to test
     * @return true if other is completely inside this AABB
     */
    bool contains(const AABB& other) const;

    /**
     * @brief Check if this AABB intersects another AABB
     * @param other Other AABB to test
     * @return true if they overlap
     */
    bool intersects(const AABB& other) const;

    /**
     * @brief Get the closest point on or inside this AABB to a given point
     * @param point Query point
     * @return Closest point on the AABB
     */
    Vector2 closestPoint(const Vector2& point) const;

    /**
     * @brief Expand this AABB to include a point
     * @param point Point to include
     */
    void expand(const Vector2& point);

    /**
     * @brief Expand this AABB to include another AABB
     * @param other AABB to include
     */
    void expand(const AABB& other);

    /**
     * @brief Get the intersection of two AABBs
     * @param other Other AABB
     * @return Intersection AABB (min if no intersection)
     */
    AABB getIntersection(const AABB& other) const;

    /**
     * @brief Translate the AABB by a vector
     * @param offset Translation offset
     */
    void translate(const Vector2& offset);

    /**
     * @brief Get a translated copy of this AABB
     * @param offset Translation offset
     * @return Translated AABB
     */
    AABB translated(const Vector2& offset) const;
};

} // namespace Math
} // namespace Engine
