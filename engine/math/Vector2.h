#pragma once
#include <cmath>

namespace Engine {
namespace Math {

/**
 * @brief 2D Vector class for positions, velocities, directions
 *
 * Used for:
 * - Entity positions (x, y)
 * - Velocities and accelerations
 * - Directions and forces
 * - 2D game physics
 */
class Vector2 {
public:
    float x, y;

    // ===== CONSTRUCTORS =====

    /**
     * @brief Default constructor - initializes to (0, 0)
     */
    Vector2();

    /**
     * @brief Construct from x and y values
     * @param x X component
     * @param y Y component
     */
    Vector2(float x, float y);

    // ===== ARITHMETIC OPERATORS =====

    /**
     * @brief Add two vectors
     * @param other The vector to add
     * @return New vector that is the sum
     */
    Vector2 operator+(const Vector2& other) const;

    /**
     * @brief Subtract two vectors
     * @param other The vector to subtract
     * @return New vector that is the difference
     */
    Vector2 operator-(const Vector2& other) const;

    /**
     * @brief Multiply vector by scalar
     * @param scalar The scalar to multiply by
     * @return New scaled vector
     */
    Vector2 operator*(float scalar) const;

    /**
     * @brief Divide vector by scalar
     * @param scalar The scalar to divide by
     * @return New scaled vector
     */
    Vector2 operator/(float scalar) const;

    // ===== COMPOUND ASSIGNMENT OPERATORS =====

    /**
     * @brief Add another vector to this vector
     * @param other The vector to add
     * @return Reference to this vector
     */
    Vector2& operator+=(const Vector2& other);

    /**
     * @brief Subtract another vector from this vector
     * @param other The vector to subtract
     * @return Reference to this vector
     */
    Vector2& operator-=(const Vector2& other);

    /**
     * @brief Multiply this vector by a scalar
     * @param scalar The scalar to multiply by
     * @return Reference to this vector
     */
    Vector2& operator*=(float scalar);

    /**
     * @brief Divide this vector by a scalar
     * @param scalar The scalar to divide by
     * @return Reference to this vector
     */
    Vector2& operator/=(float scalar);

    // ===== COMPARISON OPERATORS =====

    /**
     * @brief Check if two vectors are equal
     * @param other The vector to compare with
     * @return true if equal
     */
    bool operator==(const Vector2& other) const;

    /**
     * @brief Check if two vectors are not equal
     * @param other The vector to compare with
     * @return true if not equal
     */
    bool operator!=(const Vector2& other) const;

    // ===== VECTOR OPERATIONS =====

    /**
     * @brief Calculate dot product with another vector
     * @param other The other vector
     * @return Dot product (scalar)
     *
     * Dot product = x1*x2 + y1*y2
     * Useful for: angle between vectors, projections
     */
    float dot(const Vector2& other) const;

    /**
     * @brief Calculate the length (magnitude) of this vector
     * @return Length of the vector
     *
     * Length = sqrt(x*x + y*y)
     */
    float length() const;

    /**
     * @brief Calculate the squared length (avoids sqrt for performance)
     * @return Squared length
     *
     * Useful for distance comparisons without expensive sqrt
     */
    float lengthSquared() const;

    /**
     * @brief Normalize this vector (make length = 1)
     * Modifies this vector in place
     *
     * Normalized vector = vector / length
     * Useful for direction vectors
     */
    void normalize();

    /**
     * @brief Get a normalized copy of this vector
     * @return New normalized vector (length = 1)
     * Does not modify this vector
     */
    Vector2 normalized() const;

    // ===== STATIC HELPER FUNCTIONS =====

    /**
     * @brief Calculate distance between two points
     * @param a First point
     * @param b Second point
     * @return Distance between a and b
     */
    static float distance(const Vector2& a, const Vector2& b);

    /**
     * @brief Calculate squared distance (avoids sqrt)
     * @param a First point
     * @param b Second point
     * @return Squared distance
     */
    static float distanceSquared(const Vector2& a, const Vector2& b);

    /**
     * @brief Linear interpolation between two vectors
     * @param a Start vector
     * @param b End vector
     * @param t Interpolation factor (0.0 to 1.0)
     * @return Interpolated vector
     *
     * t=0 returns a, t=1 returns b, t=0.5 returns midpoint
     */
    static Vector2 lerp(const Vector2& a, const Vector2& b, float t);

    // ===== COMMON VECTOR CONSTANTS =====

    /**
     * @brief Zero vector (0, 0)
     */
    static const Vector2 zero;

    /**
     * @brief Unit vector pointing right (1, 0)
     */
    static const Vector2 right;

    /**
     * @brief Unit vector pointing up (0, 1)
     */
    static const Vector2 up;

    /**
     * @brief Unit vector pointing left (-1, 0)
     */
    static const Vector2 left;

    /**
     * @brief Unit vector pointing down (0, -1)
     */
    static const Vector2 down;
};

} // namespace Math
} // namespace Engine
