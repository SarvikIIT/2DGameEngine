#include "Collision.h"
#include <algorithm>
#include <cmath>

namespace Engine {
namespace Math {
    // Static methods
    bool Collision::testPointvsCircle(const Vector2& point, const Circle& circle){
        return circle.contains(point);
    }
    bool Collision::testCirclevsCircle(const Circle& a, const Circle& b){
        return a.intersects(b);
    }
    bool Collision::testAABBvsCircle(const AABB& aabb, const Circle& circle){
        return circle.intersects(aabb);
    }
    bool Collision::testAABBvsAABB(const AABB& a, const AABB& b){
        return a.intersects(b);
    }
    bool Collision::testPointvsAABB(const Vector2& point, const AABB& aabb){
        return aabb.contains(point);
    }
    bool Collision::testLinevsAABB(const Vector2& start, const Vector2& end, const AABB& aabb){
        //Liang-Barsky algorithm for line-AABB intersection
        Vector2 dir = end - start;

        float tMin = 0.0f;  // Start of line segment (at 'start')
        float tMax = 1.0f;  // End of line segment (at 'end')

        // Test against X boundaries (left and right edges)
        if(std::abs(dir.x) < 0.0001f){
            // Line is vertical (parallel to Y axis)
            // If line is outside AABB horizontally, no intersection
            if(start.x < aabb.min.x || start.x > aabb.max.x){
                return false;
            }
        } else {
            // Calculate intersection parameters with left (min.x) and right (max.x) edges
            float t1 = (aabb.min.x - start.x) / dir.x;  // Intersection with left edge
            float t2 = (aabb.max.x - start.x) / dir.x;  // Intersection with right edge

            // Ensure t1 is the smaller value
            if(t1 > t2) std::swap(t1, t2);

            // Update tMin and tMax
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            // If tMin > tMax, no intersection
            if(tMin > tMax) return false;
        }

        // Test against Y boundaries (bottom and top edges)
        if(std::abs(dir.y) < 0.0001f){
            // Line is horizontal (parallel to X axis)
            // If line is outside AABB vertically, no intersection
            if(start.y < aabb.min.y || start.y > aabb.max.y){
                return false;
            }
        } else {
            // Calculate intersection parameters with bottom (min.y) and top (max.y) edges
            float t1 = (aabb.min.y - start.y) / dir.y;  // Intersection with bottom edge
            float t2 = (aabb.max.y - start.y) / dir.y;  // Intersection with top edge

            // Ensure t1 is the smaller value
            if(t1 > t2) std::swap(t1, t2);

            // Update tMin and tMax
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            // If tMin > tMax, no intersection
            if(tMin > tMax) return false;
        }

        // If we get here, line segment intersects AABB
        // Optional: Calculate actual intersection point
        // Vector2 intersectionPoint = start + dir * tMin;

        return true;
    }

    // Line vs Circle
    bool Collision::testLinevsCircle(const Vector2& start, const Vector2& end, const Circle& circle){
        // Vector from start to circle center
        Vector2 toCenter = circle.center - start;
        Vector2 lineDir = end - start;

        // Project circle center onto line
        float lineLength = lineDir.length();
        if(lineLength < 0.0001f) return false; // Degenerate line

        Vector2 lineNorm = lineDir / lineLength;
        float projection = toCenter.dot(lineNorm);

        // Clamp projection to line segment
        projection = clamp(projection, 0.0f, lineLength);

        // Closest point on line segment to circle center
        Vector2 closestPoint = start + lineNorm * projection;

        // Check if closest point is within circle radius
        return Vector2::distanceSquared(closestPoint, circle.center) <= circle.radius * circle.radius;
    }

    // Closest point on AABB
    Vector2 Collision::closestPointOnAABB(const Vector2& point, const AABB& aabb){
        return Vector2(
            clamp(point.x, aabb.min.x, aabb.max.x),
            clamp(point.y, aabb.min.y, aabb.max.y)
        );
    }

    // Clamp utility
    float Collision::clamp(float value, float min, float max){
        if(value < min) return min;
        if(value > max) return max;
        return value;
    }

} // namespace Math
} // namespace Engine