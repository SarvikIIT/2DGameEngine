#pragma once
#include "Vector2.h"
#include "AABB.h"
#include "Circle.h"

namespace Engine {
namespace Math {

/**
 * @brief Collision detection utility functions
 *
 * Provides static functions for testing intersections between
 * different geometric primitives.
 */
class Collision {
public:
    // ===== AABB vs AABB =====

    /**
     * @brief Test if two AABBs intersect
     * @param a First AABB
     * @param b Second AABB
     * @return true if they overlap
     */
    static bool testAABBvsAABB(const AABB& a, const AABB& b);

    // ===== Circle vs Circle =====

    /**
     * @brief Test if two circles intersect
     * @param a First circle
     * @param b Second circle
     * @return true if they overlap
     */
    static bool testCirclevsCircle(const Circle& a, const Circle& b);

    // ===== AABB vs Circle =====

    /**
     * @brief Test if AABB and circle intersect
     * @param aabb AABB
     * @param circle Circle
     * @return true if they overlap
     */
    static bool testAABBvsCircle(const AABB& aabb, const Circle& circle);

    // ===== Point Tests =====

    /**
     * @brief Test if point is inside AABB
     * @param point Point to test
     * @param aabb AABB
     * @return true if point is inside
     */
    static bool testPointvsAABB(const Vector2& point, const AABB& aabb);

    /**
     * @brief Test if point is inside circle
     * @param point Point to test
     * @param circle Circle
     * @return true if point is inside
     */
    static bool testPointvsCircle(const Vector2& point, const Circle& circle);

    // ===== Line Segment Tests =====

    /**
     * @brief Test if line segment intersects AABB
     * @param start Line segment start point
     * @param end Line segment end point
     * @param aabb AABB
     * @return true if line intersects AABB
     */
    static bool testLinevsAABB(const Vector2& start, const Vector2& end, const AABB& aabb);

    /**
     * @brief Test if line segment intersects circle
     * @param start Line segment start point
     * @param end Line segment end point
     * @param circle Circle
     * @return true if line intersects circle
     */
    static bool testLinevsCircle(const Vector2& start, const Vector2& end, const Circle& circle);

    // ===== Utility Functions =====

    /**
     * @brief Get the closest point on AABB to a given point
     * @param point Point
     * @param aabb AABB
     * @return Closest point on AABB
     */
    static Vector2 closestPointOnAABB(const Vector2& point, const AABB& aabb);

    /**
     * @brief Clamp a value between min and max
     * @param value Value to clamp
     * @param min Minimum value
     * @param max Maximum value
     * @return Clamped value
     */
    static float clamp(float value, float min, float max);
};

} // namespace Math
} // namespace Engine
