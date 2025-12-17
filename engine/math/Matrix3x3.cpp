#include "Matrix3x3.h"
#include "Vector2.h"

namespace Engine {
namespace Math {
    Matrix3x3::Matrix3x3() {
        // Initialize to identity matrix
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
    }
    Matrix3x3::Matrix3x3(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22
    ) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
    }
    Matrix3x3 Matrix3x3::operator*(const Matrix3x3& other) const {
        Matrix3x3 result;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                result.m[row][col] =
                    m[row][0] * other.m[0][col] +
                    m[row][1] * other.m[1][col] +
                    m[row][2] * other.m[2][col];
            }
        }
        return result;
    }
    Vector2 Matrix3x3::operator*(const Vector2& vec) const {
        // Treat as (x, y, 1) homogeneous coordinates
        float x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * 1.0f;
        float y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * 1.0f;
        return Vector2(x, y);
    }
    Matrix3x3 Matrix3x3::translate(float x, float y) {
    return Matrix3x3(
        1, 0, x,
        0, 1, y,
        0, 0, 1
    );
    }
    Matrix3x3 Matrix3x3::rotate(float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);
    return Matrix3x3(
        c, -s, 0,
        s,  c, 0,
        0,  0, 1
    );
    }
    Matrix3x3 Matrix3x3::scale(float x, float y) {
    return Matrix3x3(
        x, 0, 0,
        0, y, 0,
        0, 0, 1
    );
    }
    Matrix3x3 Matrix3x3::TRS(const Vector2& pos, float rot, const Vector2& scl) {
    // Order: Scale * Rotate * Translate
    return translate(pos) * rotate(rot) * scale(scl.x, scl.y);
    }

    // Additional operators
    Matrix3x3 Matrix3x3::operator*(float scalar) const{
        Matrix3x3 result;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                result.m[i][j] = m[i][j] * scalar;
            }
        }
        return result;
    }
    Matrix3x3 Matrix3x3::operator+(const Matrix3x3& other) const{
        Matrix3x3 result;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                result.m[i][j] = m[i][j] + other.m[i][j];
            }
        }
        return result;
    }
    Matrix3x3 Matrix3x3::operator-(const Matrix3x3& other) const{
        Matrix3x3 result;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                result.m[i][j] = m[i][j] - other.m[i][j];
            }
        }
        return result;
    }

    // Matrix functions
    float Matrix3x3::determinant() const{
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
             - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
             + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }
    Matrix3x3 Matrix3x3::inverse() const{
        float det = determinant();
        if(det == 0.0f){
            return identity(); // Return identity if not invertible
        }
        float invDet = 1.0f / det;
        Matrix3x3 result;
        // Calculate cofactor matrix and transpose
        result.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
        result.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
        result.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;
        result.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
        result.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
        result.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;
        result.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
        result.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
        result.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;
        return result;
    }
    Matrix3x3 Matrix3x3::transpose() const{
        Matrix3x3 result;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                result.m[i][j] = m[j][i];
            }
        }
        return result;
    }

    // Static helper functions
    Matrix3x3 Matrix3x3::identity(){
        return Matrix3x3();
    }
    Matrix3x3 Matrix3x3::translate(const Vector2& translation){
        return translate(translation.x, translation.y);
    }
    Matrix3x3 Matrix3x3::scale(float scale){
        return Matrix3x3::scale(scale, scale);
    }
    Matrix3x3 Matrix3x3::scale(const Vector2& scale){
        return Matrix3x3::scale(scale.x, scale.y);
    }

} // namespace Math
} // namespace Engine