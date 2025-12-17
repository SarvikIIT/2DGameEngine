#pragma once
#include <glad/glad.h>
#include <string>

namespace Engine {
namespace Renderer {

/**
 * @brief Shader program for rendering
 *
 * Compiles vertex and fragment shaders from source code,
 * links them into a shader program, and provides utility
 * methods for setting uniforms.
 */
class Shader {
public:
    /**
     * @brief Construct shader from source code strings
     * @param vertexSource Vertex shader GLSL source code
     * @param fragmentSource Fragment shader GLSL source code
     */
    Shader(const std::string& vertexSource, const std::string& fragmentSource);

    /**
     * @brief Destructor - cleanup shader program
     */
    ~Shader();

    /**
     * @brief Use/activate this shader for rendering
     */
    void use() const;

    /**
     * @brief Get the OpenGL program ID
     */
    GLuint getID() const { return m_programID; }

    // Uniform setters
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat4(const std::string& name, const float* value) const;

private:
    GLuint m_programID;

    /**
     * @brief Compile a shader from source
     * @param source GLSL source code
     * @param type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
     * @return Compiled shader ID, or 0 on failure
     */
    GLuint compileShader(const std::string& source, GLenum type) const;

    /**
     * @brief Link vertex and fragment shaders into a program
     * @param vertexShader Compiled vertex shader ID
     * @param fragmentShader Compiled fragment shader ID
     * @return Linked program ID, or 0 on failure
     */
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) const;
};

} // namespace Renderer
} // namespace Engine
