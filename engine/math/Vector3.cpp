#include "Vector3.h"

namespace Engine {
namespace Math {
    // Constructors
    Vector3::Vector3() : x(0.0f),y(0.0f),z(0.0f) {}
    Vector3::Vector3(float x, float y, float z) : x(x),y(y),z(z) {}

    // Arithmetic Operators
    Vector3 Vector3::operator+(const Vector3& other) const{
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 Vector3::operator-(const Vector3& other) const{
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 Vector3::operator*(float scalar) const{
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    Vector3 Vector3::operator/(float scalar) const{
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    // Compound Assignment Operators
    Vector3& Vector3::operator+=(const Vector3& other){
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vector3& Vector3::operator-=(const Vector3& other){
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vector3& Vector3::operator*=(float scalar){
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    Vector3& Vector3::operator/=(float scalar){
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Comparison Operators
    bool Vector3::operator==(const Vector3& other) const{
        return (x == other.x && y == other.y && z == other.z);
    }
    bool Vector3::operator!=(const Vector3& other) const{
        return !(*this == other);
    }

    // Vector Operations
    float Vector3::dot(const Vector3& other) const{
        return x * other.x + y * other.y + z * other.z;
    }
    Vector3 Vector3::cross(const Vector3& other) const{
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    float Vector3::length() const{
        return std::sqrt(x * x + y * y + z * z);
    }
    float Vector3::lengthSquared() const{
        return x * x + y * y + z * z;
    }
    void Vector3::normalize(){
        float len = length();
        if(len > 0.0f){
            x /= len;
            y /= len;
            z /= len;
        }
    }
    Vector3 Vector3::normalized() const{
        Vector3 result = *this;
        result.normalize();
        return result;
    }

    // Static Helper Functions
    float Vector3::distance(const Vector3& a, const Vector3& b){
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float dz = b.z - a.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
    float Vector3::distanceSquared(const Vector3& a, const Vector3& b){
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float dz = b.z - a.z;
        return dx * dx + dy * dy + dz * dz;
    }
    Vector3 Vector3::lerp(const Vector3& a, const Vector3& b, float t){
        return Vector3(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t
        );
    }

    // Static Constants
    const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);
    const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::forward(0.0f, 0.0f, 1.0f);
    const Vector3 Vector3::left(-1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::down(0.0f, -1.0f, 0.0f);
    const Vector3 Vector3::back(0.0f, 0.0f, -1.0f);
} // namespace Math
} // namespace Engine
