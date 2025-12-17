#pragma once
#include <cmath>

namespace Engine {
namespace Math {

/**
 * @brief 3D Vector class for colors, 3D positions, normals
 *
 * Used for:
 * - RGB colors (r, g, b)
 * - 3D positions (x, y, z)
 * - Surface normals
 * - 3D directions
 */
class Vector3 {
public:
    float x, y, z;

    // ===== CONSTRUCTORS =====
    Vector3();
    Vector3(float x, float y, float z);

    // ===== ARITHMETIC OPERATORS =====
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    // ===== COMPOUND ASSIGNMENT OPERATORS =====
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);

    // ===== COMPARISON OPERATORS =====
    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;

    // ===== VECTOR OPERATIONS =====

    /**
     * @brief Calculate dot product with another vector
     * Dot product = x1*x2 + y1*y2 + z1*z2
     */
    float dot(const Vector3& other) const;

    /**
     * @brief Calculate cross product with another vector
     * Cross product returns a vector perpendicular to both inputs
     * Useful for finding surface normals
     */
    Vector3 cross(const Vector3& other) const;

    /**
     * @brief Calculate the length (magnitude) of this vector
     */
    float length() const;

    /**
     * @brief Calculate the squared length (avoids sqrt)
     */
    float lengthSquared() const;

    /**
     * @brief Normalize this vector (make length = 1)
     */
    void normalize();

    /**
     * @brief Get a normalized copy of this vector
     */
    Vector3 normalized() const;

    // ===== STATIC HELPER FUNCTIONS =====
    static float distance(const Vector3& a, const Vector3& b);
    static float distanceSquared(const Vector3& a, const Vector3& b);
    static Vector3 lerp(const Vector3& a, const Vector3& b, float t);

    // ===== COMMON VECTOR CONSTANTS =====
    static const Vector3 zero;
    static const Vector3 one;
    static const Vector3 right;
    static const Vector3 up;
    static const Vector3 forward;
    static const Vector3 left;
    static const Vector3 down;
    static const Vector3 back;
};

} // namespace Math
} // namespace Engine
