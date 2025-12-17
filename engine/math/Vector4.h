#pragma once
#include <cmath>

namespace Engine {
namespace Math {

/**
 * @brief 4D Vector class for RGBA colors, homogeneous coordinates
 *
 * Used for:
 * - RGBA colors (r, g, b, a)
 * - Homogeneous coordinates (x, y, z, w)
 * - 4D transformations
 */
class Vector4 {
public:
    float x, y, z, w;

    // Constructors
    Vector4();
    Vector4(float x, float y, float z, float w);

    // Arithmetic Operators
    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;
    Vector4 operator*(float scalar) const;
    Vector4 operator/(float scalar) const;

    // Compound Assignment Operators
    Vector4& operator+=(const Vector4& other);
    Vector4& operator-=(const Vector4& other);
    Vector4& operator*=(float scalar);
    Vector4& operator/=(float scalar);

    // Comparison Operators
    bool operator==(const Vector4& other) const;
    bool operator!=(const Vector4& other) const;

    // Vector Operations
    float dot(const Vector4& other) const;
    float length() const;
    float lengthSquared() const;
    void normalize();
    Vector4 normalized() const;

    // Static Helper Functions
    static float distance(const Vector4& a, const Vector4& b);
    static float distanceSquared(const Vector4& a, const Vector4& b);
    static Vector4 lerp(const Vector4& a, const Vector4& b, float t);

    // Common Constants
    static const Vector4 zero;
    static const Vector4 one;
};

} // namespace Math
} // namespace Engine
