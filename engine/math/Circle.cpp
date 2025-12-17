#include "Circle.h"
#include <algorithm>

namespace Engine {
namespace Math {

    Circle::Circle() : center(0.0f, 0.0f), radius(0.0f) {}
    Circle::Circle(const Vector2& center, float radius) : center(center), radius(radius) {}
    Circle::Circle(float x, float y, float radius) : center(x, y), radius(radius) {}
    float Circle::getArea() const {
        return 3.14159265358979323846f * radius * radius;
    }
    float Circle::getCircumference() const {
        return 2.0f * 3.14159265358979323846f * radius;
    }
    bool Circle::contains(const Vector2& point) const {
        Vector2 diff = point - center;
        return diff.dot(diff) <= radius * radius; //Distance squared comparison
    }
    bool Circle::intersects(const Circle& other) const {
        Vector2 diff = other.center - center;
        float radiusSum = radius + other.radius;
        return diff.dot(diff) <= radiusSum * radiusSum; //R1+R2<D1+D2
    }
    bool Circle::intersects(const AABB& aabb) const {
        // Find the closest point on the AABB to the circle center
        float closestX = std::max(aabb.min.x, std::min(center.x, aabb.max.x));
        float closestY = std::max(aabb.min.y, std::min(center.y, aabb.max.y));
        Vector2 closestPoint(closestX, closestY);

        // Check distance from circle center to this closest point
        Vector2 diff = closestPoint - center;
        return diff.dot(diff) <= radius * radius; //Distance squared comparison
    }

    // Distance to point
    float Circle::distanceToPoint(const Vector2& point) const{
        return Vector2::distance(center, point);
    }

    // Closest point on circle edge
    Vector2 Circle::closestPoint(const Vector2& point) const{
        Vector2 direction = (point - center).normalized();
        return center + direction * radius;
    }

    // Translate
    void Circle::translate(const Vector2& offset){
        center += offset;
    }
    Circle Circle::translated(const Vector2& offset) const{
        return Circle(center + offset, radius);
    }

    // Get bounding AABB
    AABB Circle::getBoundingAABB() const{
        return AABB(
            center.x - radius, center.y - radius,
            center.x + radius, center.y + radius
        );
    }

} // namespace Math
} // namespace Engine