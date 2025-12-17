#include "Matrix4x4.h"

namespace Engine {
namespace Math {
    // Constructors
    Matrix4x4::Matrix4x4(){
        // Identity matrix
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }
    Matrix4x4::Matrix4x4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    ){
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
    }

    // Matrix Operations
    Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const{
        Matrix4x4 result;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                result.m[i][j] = 0;
                for(int k = 0; k < 4; k++){
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
    Vector4 Matrix4x4::operator*(const Vector4& vec) const{
        return Vector4(
            m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3] * vec.w,
            m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3] * vec.w,
            m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3] * vec.w,
            m[3][0] * vec.x + m[3][1] * vec.y + m[3][2] * vec.z + m[3][3] * vec.w
        );
    }
    Vector3 Matrix4x4::transformPoint(const Vector3& point) const{
        Vector4 result = (*this) * Vector4(point.x, point.y, point.z, 1.0f);
        return Vector3(result.x, result.y, result.z);
    }
    Vector3 Matrix4x4::transformDirection(const Vector3& dir) const{
        Vector4 result = (*this) * Vector4(dir.x, dir.y, dir.z, 0.0f);
        return Vector3(result.x, result.y, result.z);
    }
    Matrix4x4 Matrix4x4::operator*(float scalar) const{
        Matrix4x4 result;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                result.m[i][j] = m[i][j] * scalar;
            }
        }
        return result;
    }
    Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const{
        Matrix4x4 result;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                result.m[i][j] = m[i][j] + other.m[i][j];
            }
        }
        return result;
    }
    Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const{
        Matrix4x4 result;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                result.m[i][j] = m[i][j] - other.m[i][j];
            }
        }
        return result;
    }

    // Matrix Functions
    float Matrix4x4::determinant() const{
        // Using cofactor expansion (simplified)
        float a = m[0][0], b = m[0][1], c = m[0][2], d = m[0][3];
        float e = m[1][0], f = m[1][1], g = m[1][2], h = m[1][3];
        float i = m[2][0], j = m[2][1], k = m[2][2], l = m[2][3];
        float mm = m[3][0], n = m[3][1], o = m[3][2], p = m[3][3];

        return a * (f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n))
             - b * (e * (k * p - l * o) - g * (i * p - l * mm) + h * (i * o - k * mm))
             + c * (e * (j * p - l * n) - f * (i * p - l * mm) + h * (i * n - j * mm))
             - d * (e * (j * o - k * n) - f * (i * o - k * mm) + g * (i * n - j * mm));
    }
    Matrix4x4 Matrix4x4::inverse() const{
        float det = determinant();
        if(det == 0.0f){
            return identity();
        }
        // Using adjugate method (simplified implementation)
        // For production, use optimized inverse calculation
        Matrix4x4 result;
        float invDet = 1.0f / det;

        // Calculate cofactor matrix (simplified - full implementation would be longer)
        // This is a basic implementation
        result.m[0][0] = (m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2]) * invDet;
        result.m[0][1] = (-m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2]) * invDet;
        result.m[0][2] = (m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2]) * invDet;
        result.m[0][3] = (-m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2]) * invDet;

        result.m[1][0] = (-m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2]) * invDet;
        result.m[1][1] = (m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2]) * invDet;
        result.m[1][2] = (-m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2]) * invDet;
        result.m[1][3] = (m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2]) * invDet;

        result.m[2][0] = (m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1]) * invDet;
        result.m[2][1] = (-m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1]) * invDet;
        result.m[2][2] = (m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1]) * invDet;
        result.m[2][3] = (-m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1]) * invDet;

        result.m[3][0] = (-m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1]) * invDet;
        result.m[3][1] = (m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1]) * invDet;
        result.m[3][2] = (-m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1]) * invDet;
        result.m[3][3] = (m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1]) * invDet;

        return result;
    }
    Matrix4x4 Matrix4x4::transpose() const{
        Matrix4x4 result;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                result.m[i][j] = m[j][i];
            }
        }
        return result;
    }

    // Static Transformation Matrices
    Matrix4x4 Matrix4x4::identity(){
        return Matrix4x4();
    }
    Matrix4x4 Matrix4x4::translate(float x, float y, float z){
        return Matrix4x4(
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1
        );
    }
    Matrix4x4 Matrix4x4::translate(const Vector3& translation){
        return translate(translation.x, translation.y, translation.z);
    }
    Matrix4x4 Matrix4x4::rotateX(float angle){
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Matrix4x4(
            1, 0,  0, 0,
            0, c, -s, 0,
            0, s,  c, 0,
            0, 0,  0, 1
        );
    }
    Matrix4x4 Matrix4x4::rotateY(float angle){
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Matrix4x4(
             c, 0, s, 0,
             0, 1, 0, 0,
            -s, 0, c, 0,
             0, 0, 0, 1
        );
    }
    Matrix4x4 Matrix4x4::rotateZ(float angle){
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Matrix4x4(
            c, -s, 0, 0,
            s,  c, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        );
    }
    Matrix4x4 Matrix4x4::scale(float x, float y, float z){
        return Matrix4x4(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1
        );
    }
    Matrix4x4 Matrix4x4::scale(float scale){
        return Matrix4x4::scale(scale, scale, scale);
    }
    Matrix4x4 Matrix4x4::scale(const Vector3& scale){
        return Matrix4x4::scale(scale.x, scale.y, scale.z);
    }
    Matrix4x4 Matrix4x4::TRS(const Vector3& position, const Vector3& rotation, const Vector3& scale){
        return translate(position) * rotateZ(rotation.z) * rotateY(rotation.y) * rotateX(rotation.x) * Matrix4x4::scale(scale);
    }

    // Camera Matrices : Written Completely by AI I know nothing about cameras :)
    Matrix4x4 Matrix4x4::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up){
        Vector3 zAxis = (eye - target).normalized();
        Vector3 xAxis = up.cross(zAxis).normalized();
        Vector3 yAxis = zAxis.cross(xAxis);

        return Matrix4x4(
            xAxis.x, xAxis.y, xAxis.z, -xAxis.dot(eye),
            yAxis.x, yAxis.y, yAxis.z, -yAxis.dot(eye),
            zAxis.x, zAxis.y, zAxis.z, -zAxis.dot(eye),
            0, 0, 0, 1
        );
    }
    Matrix4x4 Matrix4x4::perspective(float fov, float aspect, float near, float far){
        float tanHalfFov = std::tan(fov / 2.0f);
        return Matrix4x4(
            1.0f / (aspect * tanHalfFov), 0, 0, 0,
            0, 1.0f / tanHalfFov, 0, 0,
            0, 0, -(far + near) / (far - near), -(2.0f * far * near) / (far - near),
            0, 0, -1, 0
        );
    }
    Matrix4x4 Matrix4x4::orthographic(float left, float right, float bottom, float top, float near, float far){
        return Matrix4x4(
            2.0f / (right - left), 0, 0, -(right + left) / (right - left),
            0, 2.0f / (top - bottom), 0, -(top + bottom) / (top - bottom),
            0, 0, -2.0f / (far - near), -(far + near) / (far - near),
            0, 0, 0, 1
        );
    }
} // namespace Math
} // namespace Engine
