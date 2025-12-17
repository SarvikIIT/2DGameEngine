#include "Vector4.h"

namespace Engine {
namespace Math {
    // Constructors
    Vector4::Vector4() : x(0.0f),y(0.0f),z(0.0f),w(0.0f) {}
    Vector4::Vector4(float x, float y, float z, float w) : x(x),y(y),z(z),w(w) {}

    // Arithmetic Operators
    Vector4 Vector4::operator+(const Vector4& other) const{
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    Vector4 Vector4::operator-(const Vector4& other) const{
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    Vector4 Vector4::operator*(float scalar) const{
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    Vector4 Vector4::operator/(float scalar) const{
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    // Compound Assignment Operators
    Vector4& Vector4::operator+=(const Vector4& other){
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    Vector4& Vector4::operator-=(const Vector4& other){
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    Vector4& Vector4::operator*=(float scalar){
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }
    Vector4& Vector4::operator/=(float scalar){
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    // Comparison Operators
    bool Vector4::operator==(const Vector4& other) const{
        return (x == other.x && y == other.y && z == other.z && w == other.w);
    }
    bool Vector4::operator!=(const Vector4& other) const{
        return !(*this == other);
    }

    // Vector Operations
    float Vector4::dot(const Vector4& other) const{
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    float Vector4::length() const{
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    float Vector4::lengthSquared() const{
        return x * x + y * y + z * z + w * w;
    }
    void Vector4::normalize(){
        float len = length();
        if(len > 0.0f){
            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }
    }
    Vector4 Vector4::normalized() const{
        Vector4 result = *this;
        result.normalize();
        return result;
    }

    // Static Helper Functions
    float Vector4::distance(const Vector4& a, const Vector4& b){
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float dz = b.z - a.z;
        float dw = b.w - a.w;
        return std::sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
    }
    float Vector4::distanceSquared(const Vector4& a, const Vector4& b){
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float dz = b.z - a.z;
        float dw = b.w - a.w;
        return dx * dx + dy * dy + dz * dz + dw * dw;
    }
    Vector4 Vector4::lerp(const Vector4& a, const Vector4& b, float t){
        return Vector4(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t,
            a.w + (b.w - a.w) * t
        );
    }

    // Static Constants
    const Vector4 Vector4::zero(0.0f, 0.0f, 0.0f, 0.0f);
    const Vector4 Vector4::one(1.0f, 1.0f, 1.0f, 1.0f);
} // namespace Math
} // namespace Engine
