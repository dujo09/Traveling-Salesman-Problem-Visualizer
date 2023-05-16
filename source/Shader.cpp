#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShaderId, fragmentShaderId;
    int success;
    char infoLog[512];

    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vShaderCode, NULL);
    glCompileShader(vertexShaderId);
    checkCompileErrors(vertexShaderId, "VERTEX");

    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShaderId);
    checkCompileErrors(fragmentShaderId, "FRAGMENT");

    m_shaderProgramId = glCreateProgram();
    glAttachShader(m_shaderProgramId, vertexShaderId);
    glAttachShader(m_shaderProgramId, fragmentShaderId);
    glLinkProgram(m_shaderProgramId);
    checkCompileErrors(m_shaderProgramId, "PROGRAM");

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

Shader::Shader(const char* vertexShaderPath, const char* geometryShaderPath, const char* fragmentShaderPath)
{
    std::string vertexCode;
    std::string geometryCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream gShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexShaderPath);
        gShaderFile.open(geometryShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, gShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        gShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        geometryCode = gShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShaderId, geometryShaderId, fragmentShaderId;
    int success;
    char infoLog[512];

    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vShaderCode, NULL);
    glCompileShader(vertexShaderId);
    checkCompileErrors(vertexShaderId, "VERTEX");

    geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShaderId, 1, &gShaderCode, NULL);
    glCompileShader(geometryShaderId);
    checkCompileErrors(geometryShaderId, "GEOMETRY");

    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShaderId);
    checkCompileErrors(fragmentShaderId, "FRAGMENT");

    m_shaderProgramId = glCreateProgram();
    glAttachShader(m_shaderProgramId, vertexShaderId);
    glAttachShader(m_shaderProgramId, geometryShaderId);
    glAttachShader(m_shaderProgramId, fragmentShaderId);
    glLinkProgram(m_shaderProgramId);
    checkCompileErrors(m_shaderProgramId, "PROGRAM");

    glDeleteShader(vertexShaderId);
    glDeleteShader(geometryShaderId);
    glDeleteShader(fragmentShaderId);
}

Shader::~Shader() {
    glDeleteProgram(m_shaderProgramId);
}

void Shader::use() {
    glUseProgram(m_shaderProgramId);
}

void Shader::setBool(const char* name, bool value) const 
{
    glUniform1i(glGetUniformLocation(m_shaderProgramId, name), (int)value);
}

void Shader::setInt(const char* name, int value) const 
{
    glUniform1i(glGetUniformLocation(m_shaderProgramId, name), value);
}

void Shader::setFloat(const char* name, float value) const 
{
    glUniform1f(glGetUniformLocation(m_shaderProgramId, name), value);
}

void Shader::setVec2(const char* name, const glm::vec2& value) const 
{
    glUniform2fv(glGetUniformLocation(m_shaderProgramId, name), 1, &value[0]);
}
void Shader::setVec2(const char* name, float x, float y) const 
{
    glUniform2f(glGetUniformLocation(m_shaderProgramId, name), x, y);
}

void Shader::setVec3(const char* name, const glm::vec3& value) const 
{
    glUniform3fv(glGetUniformLocation(m_shaderProgramId, name), 1, &value[0]);
}
void Shader::setVec3(const char* name, float x, float y, float z) const 
{
    glUniform3f(glGetUniformLocation(m_shaderProgramId, name), x, y, z);
}

void Shader::setVec4(const char* name, const glm::vec4& value) const 
{
    glUniform4fv(glGetUniformLocation(m_shaderProgramId, name), 1, &value[0]);
}
void Shader::setVec4(const char* name, float x, float y, float z, float w) const 
{
    glUniform4f(glGetUniformLocation(m_shaderProgramId, name), x, y, z, w);
}

void Shader::setMat2(const char* name, const glm::mat2& mat) const 
{
    glUniformMatrix2fv(glGetUniformLocation(m_shaderProgramId, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const char* name, const glm::mat3& mat) const 
{
    glUniformMatrix3fv(glGetUniformLocation(m_shaderProgramId, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const char* name, const glm::mat4& mat) const 
{
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramId, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shaderId, std::string type) 
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n" << std::endl;
        }
    } else {
        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderId, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n" << std::endl;
        }
    }
}
