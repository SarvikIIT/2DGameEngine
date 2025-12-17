#pragma once
#include "Vector2.h"
#include <cmath>

namespace Engine {
namespace Math {

/**
 * @brief 2x2 Matrix for simple 2D rotations and scales
 *
 * Matrix layout:
 * | m00  m01 |
 * | m10  m11 |
 */
class Matrix2x2 {
public:
    float m[2][2];

    // Constructors
    Matrix2x2();
    Matrix2x2(float m00, float m01, float m10, float m11);

    // Matrix Operations
    Matrix2x2 operator*(const Matrix2x2& other) const;
    Vector2 operator*(const Vector2& vec) const;
    Matrix2x2 operator*(float scalar) const;
    Matrix2x2 operator+(const Matrix2x2& other) const;
    Matrix2x2 operator-(const Matrix2x2& other) const;

    // Matrix Functions
    float determinant() const;
    Matrix2x2 inverse() const;
    Matrix2x2 transpose() const;

    // Static Transformation Matrices
    static Matrix2x2 identity();
    static Matrix2x2 rotate(float angle);
    static Matrix2x2 scale(float x, float y);
    static Matrix2x2 scale(float scale);
    static Matrix2x2 scale(const Vector2& scale);
};

} // namespace Math
} // namespace Engine
