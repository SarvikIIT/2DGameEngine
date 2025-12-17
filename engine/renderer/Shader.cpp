#include "Shader.h"
#include <SDL3/SDL.h>
#include<vector>

namespace Engine{
namespace Renderer{
    //Constructor:
    Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
    : m_programID(0)
    {
        GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER); // Creates shapes using triangles
        GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER); // Colors the shapes

        if (vertexShader == 0 || fragmentShader == 0) {
            SDL_Log("Shader compilation failed.");
            return;
        }

        m_programID = linkProgram(vertexShader, fragmentShader);

        // Cleanup compiled shaders after linking
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (m_programID == 0) {
            SDL_Log("Shader program linking failed.");
        }
    }
    Shader::~Shader(){
        if(m_programID != 0){
            glDeleteProgram(m_programID);
        }
    }
    GLuint Shader::compileShader(const std::string& source, GLenum type) const{
        //Create shader object and take its ID:
        GLuint shader = glCreateShader(type);
        //Give OpenGL the source code:
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        //Compile the shader:
        glCompileShader(shader);

        //Check for errors:
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            GLint logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<char> log(logLength);
            glGetShaderInfoLog(shader, logLength, nullptr, log.data());
            SDL_Log("Shader error: %s", log.data());
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    GLuint Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) const{
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        //Check linking errors:
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success){
            GLint logLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<char> log(logLength);
            glGetProgramInfoLog(program, logLength, nullptr, log.data());
            SDL_Log("Shader linking error: %s", log.data());
            glDeleteProgram(program);
            return 0;
        }

        return program;
    }

    void Shader::use() const{
        glUseProgram(m_programID);
    }

    //Uniform setters:
    void Shader::setInt(const std::string& name, int value) const{
        glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value) const{
        glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
    }

    void Shader::setVec2(const std::string& name, float x, float y) const{
        glUniform2f(glGetUniformLocation(m_programID, name.c_str()), x, y);
    }

    void Shader::setVec3(const std::string& name, float x, float y, float z) const{
        glUniform3f(glGetUniformLocation(m_programID, name.c_str()), x, y, z);
    }

    void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const{
        glUniform4f(glGetUniformLocation(m_programID, name.c_str()), x, y, z, w);
    }

    void Shader::setMat4(const std::string& name, const float* value) const{
        glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_TRUE, value);
    }

} // namespace Renderer
} // namespace Engine