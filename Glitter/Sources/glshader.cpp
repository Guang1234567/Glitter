
// Local Headers
#include "glshader.hpp"

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>

int checkCompileErrors(GLuint shader, std::string type);

GLShader::GLShader(const GLchar *vertexShaderSrcFilePath,
         const GLchar *fragmentShaderSrcFilePath, int* pSuccess) {

    // 1. retrieve the vertex/fragment source code from filePath
    // ---------------------------------------------------------
    
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexShaderSrcFilePath);
        fShaderFile.open(fragmentShaderSrcFilePath);
        
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::exception& e) {
      std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl
                << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"
        << std::endl
        << "Unknown Exception"
        << std::endl;
    }
    
    // convert string into c-style-string
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    
    
    // 2. compile shaders
    // ---------------------------------------------------------
    
    // vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    if(NULL != pSuccess) {
        *pSuccess = checkCompileErrors(vertex, "VERTEX");
    }
    
    // fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    if(NULL != pSuccess) {
        *pSuccess = checkCompileErrors(fragment, "FRAGMENT");
    }
    
    // shader program
    mId = glCreateProgram();
    glAttachShader(mId, vertex);
    glAttachShader(mId, fragment);
    glLinkProgram(mId);
    if(NULL != pSuccess) {
        *pSuccess = checkCompileErrors(mId, "PROGRAM");
    }

    // delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void GLShader::use() {
    glUseProgram(mId);
}

bool GLShader::setBool(const std::string &name, bool value) const {
    GLint idx = glGetUniformLocation(mId, name.c_str());
    if (-1 != idx) {
        glUniform1i(idx, (int)value);
        return true;
    }
    return false;
}

bool GLShader::setInt(const std::string &name, int value) const
{
    GLint idx = glGetUniformLocation(mId, name.c_str());
    if (-1 != idx) {
        glUniform1i(idx, value);
        return true;
    }
    return false;
}

bool GLShader::setFloat(const std::string &name, float value) const
{
    GLint idx = glGetUniformLocation(mId, name.c_str());
    if (-1 != idx) {
        glUniform1f(idx, value);
        return true;
    }
    return false;
}

bool GLShader::setFloat(const std::string &name, float x, float y, float z) const
{
    GLint idx = glGetUniformLocation(mId, name.c_str());
    if (-1 != idx) {
      glUniform3f(idx, x, y, z);
      return true;
    }
    return false;
}

bool GLShader::setFloat(const std::string &name, float x, float y, float z, float w) const
{
    GLint idx = glGetUniformLocation(mId, name.c_str());
    if (-1 != idx) {
      glUniform4f(idx, x, y, z, w);
      return true;
    }
    return false;
}

int checkCompileErrors(GLuint shader, std::string type)
{
    int success = 0;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (0 == success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (0 == success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    
    return success;
}
