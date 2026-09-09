#include "AABB.h"
#include <algorithm>

namespace Engine {
namespace Math {
    Vector2 AABB::getCenter() const {
        return Vector2(
            (min.x + max.x) * 0.5f,
            (min.y + max.y) * 0.5f
        );
    }
    Vector2 AABB::getSize() const {
        return Vector2(
            max.x - min.x,
            max.y - min.y
        );
    }
    float AABB::getWidth() const {
        return max.x - min.x;
    }

    bool AABB::intersects(const AABB& other) const {
        if(max.x < other.min.x || min.x > other.max.x) return false; // Completely to the left or right
        if(max.y < other.min.y || min.y > other.max.y) return false; // Completely above or below
        return true;
    }
    bool AABB::contains(const Vector2& point) const {
        return (point.x >= min.x && point.x <= max.x &&
                point.y >= min.y && point.y <= max.y);
    }

    bool AABB::contains(const AABB& other) const {
        return (other.min.x >= min.x && other.max.x <= max.x &&
                other.min.y >= min.y && other.max.y <= max.y);
    }

    Vector2 AABB::closestPoint(const Vector2& point) const {
        return Vector2(
            std::max(min.x, std::min(point.x, max.x)),
            std::max(min.y, std::min(point.y, max.y))
        );
    }
    Vector2 AABB::getHalfExtents() const {
        return Vector2(
            (max.x - min.x) * 0.5f,
            (max.y - min.y) * 0.5f
        );
    }
    float AABB::getHeight() const {
        return max.y - min.y;
    }

    // Constructors
    AABB::AABB() : min(0.0f, 0.0f), max(0.0f, 0.0f) {}
    AABB::AABB(const Vector2& min, const Vector2& max) : min(min), max(max) {}
    AABB::AABB(float minX, float minY, float maxX, float maxY) : min(minX, minY), max(maxX, maxY) {}

    // Static factory method
    AABB AABB::fromCenterSize(const Vector2& center, const Vector2& size){
        Vector2 halfSize = size * 0.5f;
        return AABB(center - halfSize, center + halfSize);
    }

    // Expand methods
    void AABB::expand(const Vector2& point){
        if(point.x < min.x) min.x = point.x;
        if(point.y < min.y) min.y = point.y;
        if(point.x > max.x) max.x = point.x;
        if(point.y > max.y) max.y = point.y;
    }
    void AABB::expand(const AABB& other){
        if(other.min.x < min.x) min.x = other.min.x;
        if(other.min.y < min.y) min.y = other.min.y;
        if(other.max.x > max.x) max.x = other.max.x;
        if(other.max.y > max.y) max.y = other.max.y;
    }

    // Get intersection
    AABB AABB::getIntersection(const AABB& other) const{
        if(!intersects(other)){
            return AABB(); // Return empty AABB if no intersection
        }
        return AABB(
            Vector2(
                std::max(min.x, other.min.x),
                std::max(min.y, other.min.y)
            ),
            Vector2(
                std::min(max.x, other.max.x),
                std::min(max.y, other.max.y)
            )
        );
    }

    // Translate methods
    void AABB::translate(const Vector2& offset){
        min += offset;
        max += offset;
    }
    AABB AABB::translated(const Vector2& offset) const{
        return AABB(min + offset, max + offset);
    }

} // namespace Math
} // namespace Engine