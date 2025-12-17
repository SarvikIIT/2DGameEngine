#include "Vector2.h"

namespace Engine {
namespace Math {
    // Constructors
    Vector2::Vector2() : x(0.0f),y(0.0f) {}
    Vector2::Vector2(float x, float y) : x(x),y(y) {}

    // Arithmetic Operators
    Vector2 Vector2::operator+(const Vector2& other) const{
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 Vector2::operator-(const Vector2& other) const{
        return Vector2(x - other.x, y - other.y);
    }
    Vector2 Vector2::operator*(float scalar) const{
        return Vector2(x * scalar, y * scalar);
    }
    Vector2 Vector2::operator/(float scalar) const{
        return Vector2(x / scalar, y / scalar);
    }

    // Compound Assignment Operators
    Vector2& Vector2::operator+=(const Vector2& other){
        x += other.x;
        y += other.y;
        return *this;
    }
    Vector2& Vector2::operator-=(const Vector2& other){
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vector2& Vector2::operator*=(float scalar){
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vector2& Vector2::operator/=(float scalar){
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Comparison Operators
    bool Vector2::operator==(const Vector2& other) const{
        return (x == other.x && y == other.y);
    }
    bool Vector2::operator!=(const Vector2& other) const{
        return !(*this == other);
    }

    // Vector Operations
    float Vector2::dot(const Vector2& other) const{
        return x * other.x + y * other.y;
    }
    float Vector2::length() const{
        return std::sqrt(x * x + y * y);
    }
    float Vector2::lengthSquared() const{
        return x * x + y * y;
    }
    void Vector2::normalize(){
        float len = length();
        if(len > 0.0f){
            x /= len;
            y /= len;
        }
    }
    Vector2 Vector2::normalized() const{
        Vector2 result = *this;
        result.normalize();
        return result;
    }

    // Static Helper Functions
    float Vector2::distance(const Vector2& a, const Vector2& b){
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    float Vector2::distanceSquared(const Vector2& a, const Vector2& b){
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return dx * dx + dy * dy;
    }
    Vector2 Vector2::lerp(const Vector2& a, const Vector2& b, float t){
        return Vector2(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t
        );
    }

    // Static Constants
    const Vector2 Vector2::zero(0.0f, 0.0f);
    const Vector2 Vector2::right(1.0f, 0.0f);
    const Vector2 Vector2::up(0.0f, 1.0f);
    const Vector2 Vector2::left(-1.0f, 0.0f);
    const Vector2 Vector2::down(0.0f, -1.0f);
} // namespace Math
} // namespace Engine
