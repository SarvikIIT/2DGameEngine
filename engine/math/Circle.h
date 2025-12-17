#pragma once
#include "Vector2.h"
#include "AABB.h"

namespace Engine {
namespace Math {

// Forward declaration
class AABB;

/**
 * @brief Circle for 2D collision detection
 *
 * Defined by center point and radius.
 * Used for circular collision bounds (enemies, bullets, etc.)
 */
class Circle {
public:
    Vector2 center;
    float radius;

    // Constructors
    Circle();
    Circle(const Vector2& center, float radius);
    Circle(float x, float y, float radius);

    /**
     * @brief Get the area of the circle
     * @return Area (π * r²)
     */
    float getArea() const;

    /**
     * @brief Get the circumference of the circle
     * @return Circumference (2 * π * r)
     */
    float getCircumference() const;

    /**
     * @brief Check if a point is inside this circle
     * @param point Point to test
     * @return true if point is inside
     */
    bool contains(const Vector2& point) const;

    /**
     * @brief Check if this circle intersects another circle
     * @param other Other circle to test
     * @return true if they overlap
     */
    bool intersects(const Circle& other) const;

    /**
     * @brief Check if this circle intersects an AABB
     * @param aabb AABB to test
     * @return true if they overlap
     */
    bool intersects(const AABB& aabb) const;

    /**
     * @brief Get distance from circle center to point
     * @param point Point to measure to
     * @return Distance
     */
    float distanceToPoint(const Vector2& point) const;

    /**
     * @brief Get the closest point on the circle to a given point
     * @param point Point to find closest point to
     * @return Closest point on circle edge
     */
    Vector2 closestPoint(const Vector2& point) const;

    /**
     * @brief Translate the circle by a vector
     * @param offset Translation offset
     */
    void translate(const Vector2& offset);

    /**
     * @brief Get a translated copy of this circle
     * @param offset Translation offset
     * @return Translated circle
     */
    Circle translated(const Vector2& offset) const;

    /**
     * @brief Get an AABB that bounds this circle
     * @return Bounding AABB
     */
    AABB getBoundingAABB() const;
};

} // namespace Math
} // namespace Engine
