#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <cmath>

namespace Engine {
namespace Math {

/**
 * @brief 3x3 Matrix for 2D transformations
 *
 * Used for:
 * - 2D transformations (translate, rotate, scale)
 * - Sprite positioning and rotation
 * - Camera transformations
 *
 * Matrix layout (row-major):
 * | m00  m01  m02 |
 * | m10  m11  m12 |
 * | m20  m21  m22 |
 *
 * For 2D transformations:
 * | scale_x * cos(θ)   -sin(θ)    translate_x |
 * | sin(θ)              scale_y * cos(θ)    translate_y |
 * | 0                   0                    1           |
 */
class Matrix3x3 {
public:
    // Matrix elements (row-major: m[row][col])
    float m[3][3];

    // ===== CONSTRUCTORS =====

    /**
     * @brief Default constructor - creates identity matrix
     */
    Matrix3x3();

    /**
     * @brief Construct from 9 values (row-major order)
     */
    Matrix3x3(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22
    );

    // ===== MATRIX OPERATIONS =====

    /**
     * @brief Multiply two matrices
     * @param other The matrix to multiply with
     * @return Result of matrix multiplication
     */
    Matrix3x3 operator*(const Matrix3x3& other) const;

    /**
     * @brief Transform a 2D vector (treats as homogeneous with w=1)
     * @param vec The vector to transform
     * @return Transformed vector
     */
    Vector2 operator*(const Vector2& vec) const;

    /**
     * @brief Multiply matrix by scalar
     * @param scalar The scalar to multiply by
     * @return Scaled matrix
     */
    Matrix3x3 operator*(float scalar) const;

    /**
     * @brief Add two matrices
     * @param other The matrix to add
     * @return Sum of matrices
     */
    Matrix3x3 operator+(const Matrix3x3& other) const;

    /**
     * @brief Subtract two matrices
     * @param other The matrix to subtract
     * @return Difference of matrices
     */
    Matrix3x3 operator-(const Matrix3x3& other) const;

    // ===== MATRIX FUNCTIONS =====

    /**
     * @brief Calculate the determinant of the matrix
     * @return Determinant value
     */
    float determinant() const;

    /**
     * @brief Calculate the inverse of the matrix
     * @return Inverted matrix
     * Note: If determinant is 0, returns identity matrix
     */
    Matrix3x3 inverse() const;

    /**
     * @brief Transpose the matrix (swap rows and columns)
     * @return Transposed matrix
     */
    Matrix3x3 transpose() const;

    // ===== STATIC TRANSFORMATION MATRICES =====

    /**
     * @brief Create identity matrix
     * @return Identity matrix (diagonal 1s)
     */
    static Matrix3x3 identity();

    /**
     * @brief Create translation matrix
     * @param x Translation in x direction
     * @param y Translation in y direction
     * @return Translation matrix
     */
    static Matrix3x3 translate(float x, float y);

    /**
     * @brief Create translation matrix from vector
     * @param translation Translation vector
     * @return Translation matrix
     */
    static Matrix3x3 translate(const Vector2& translation);

    /**
     * @brief Create rotation matrix
     * @param angle Rotation angle in radians
     * @return Rotation matrix
     */
    static Matrix3x3 rotate(float angle);

    /**
     * @brief Create scale matrix
     * @param x Scale in x direction
     * @param y Scale in y direction
     * @return Scale matrix
     */
    static Matrix3x3 scale(float x, float y);

    /**
     * @brief Create uniform scale matrix
     * @param scale Uniform scale factor
     * @return Scale matrix
     */
    static Matrix3x3 scale(float scale);

    /**
     * @brief Create scale matrix from vector
     * @param scale Scale vector
     * @return Scale matrix
     */
    static Matrix3x3 scale(const Vector2& scale);

    /**
     * @brief Create a full 2D transformation matrix
     * @param position Translation
     * @param rotation Rotation angle in radians
     * @param scale Scale factors
     * @return Combined transformation matrix (TRS order)
     *
     * Order: First Scale, then Rotate, then Translate
     */
    static Matrix3x3 TRS(const Vector2& position, float rotation, const Vector2& scale);
};

} // namespace Math
} // namespace Engine
