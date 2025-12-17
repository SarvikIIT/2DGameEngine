#include "Matrix2x2.h"

namespace Engine {
namespace Math {
    // Constructors
    Matrix2x2::Matrix2x2(){
        m[0][0] = 1.0f; m[0][1] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f;
    }
    Matrix2x2::Matrix2x2(float m00, float m01, float m10, float m11){
        m[0][0] = m00; m[0][1] = m01;
        m[1][0] = m10; m[1][1] = m11;
    }

    // Matrix Operations
    Matrix2x2 Matrix2x2::operator*(const Matrix2x2& other) const{
        Matrix2x2 result;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                result.m[i][j] = 0;
                for(int k = 0; k < 2; k++){
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
    Vector2 Matrix2x2::operator*(const Vector2& vec) const{
        return Vector2(
            m[0][0] * vec.x + m[0][1] * vec.y,
            m[1][0] * vec.x + m[1][1] * vec.y
        );
    }
    Matrix2x2 Matrix2x2::operator*(float scalar) const{
        Matrix2x2 result;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                result.m[i][j] = m[i][j] * scalar;
            }
        }
        return result;
    }
    Matrix2x2 Matrix2x2::operator+(const Matrix2x2& other) const{
        Matrix2x2 result;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                result.m[i][j] = m[i][j] + other.m[i][j];
            }
        }
        return result;
    }
    Matrix2x2 Matrix2x2::operator-(const Matrix2x2& other) const{
        Matrix2x2 result;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                result.m[i][j] = m[i][j] - other.m[i][j];
            }
        }
        return result;
    }

    // Matrix Functions
    float Matrix2x2::determinant() const{
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }
    Matrix2x2 Matrix2x2::inverse() const{
        float det = determinant();
        if(det == 0.0f){
            return identity();
        }
        float invDet = 1.0f / det;
        return Matrix2x2(
            m[1][1] * invDet, -m[0][1] * invDet,
            -m[1][0] * invDet, m[0][0] * invDet
        );
    }
    Matrix2x2 Matrix2x2::transpose() const{
        return Matrix2x2(
            m[0][0], m[1][0],
            m[0][1], m[1][1]
        );
    }

    // Static Functions
    Matrix2x2 Matrix2x2::identity(){
        return Matrix2x2();
    }
    Matrix2x2 Matrix2x2::rotate(float angle){
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Matrix2x2(c, -s, s, c);
    }
    Matrix2x2 Matrix2x2::scale(float x, float y){
        return Matrix2x2(x, 0, 0, y);
    }
    Matrix2x2 Matrix2x2::scale(float scale){
        return Matrix2x2::scale(scale, scale);
    }
    Matrix2x2 Matrix2x2::scale(const Vector2& scale){
        return Matrix2x2::scale(scale.x, scale.y);
    }
} // namespace Math
} // namespace Engine
