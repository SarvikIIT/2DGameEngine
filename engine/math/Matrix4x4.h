#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <cmath>

namespace Engine {
namespace Math {

/**
 * @brief 4x4 Matrix for 3D transformations
 *
 * Used for:
 * - 3D transformations (translate, rotate, scale)
 * - Camera view and projection matrices
 * - OpenGL/DirectX transformations
 *
 * Matrix layout (row-major):
 * | m00  m01  m02  m03 |
 * | m10  m11  m12  m13 |
 * | m20  m21  m22  m23 |
 * | m30  m31  m32  m33 |
 */
class Matrix4x4 {
public:
    float m[4][4];

    // Constructors
    Matrix4x4();
    Matrix4x4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    );

    // Matrix Operations
    Matrix4x4 operator*(const Matrix4x4& other) const;
    Vector4 operator*(const Vector4& vec) const;
    Vector3 transformPoint(const Vector3& point) const;
    Vector3 transformDirection(const Vector3& dir) const;
    Matrix4x4 operator*(float scalar) const;
    Matrix4x4 operator+(const Matrix4x4& other) const;
    Matrix4x4 operator-(const Matrix4x4& other) const;

    // Matrix Functions
    float determinant() const;
    Matrix4x4 inverse() const;
    Matrix4x4 transpose() const;

    // Get raw data pointer (for OpenGL)
    const float* data() const { return &m[0][0]; }

    // Static Transformation Matrices
    static Matrix4x4 identity();
    static Matrix4x4 translate(float x, float y, float z);
    static Matrix4x4 translate(const Vector3& translation);
    static Matrix4x4 rotateX(float angle);
    static Matrix4x4 rotateY(float angle);
    static Matrix4x4 rotateZ(float angle);
    static Matrix4x4 scale(float x, float y, float z);
    static Matrix4x4 scale(float scale);
    static Matrix4x4 scale(const Vector3& scale);
    static Matrix4x4 TRS(const Vector3& position, const Vector3& rotation, const Vector3& scale);

    // Camera Matrices
    static Matrix4x4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
    static Matrix4x4 perspective(float fov, float aspect, float near, float far);
    static Matrix4x4 orthographic(float left, float right, float bottom, float top, float near, float far);
};

} // namespace Math
} // namespace Engine
