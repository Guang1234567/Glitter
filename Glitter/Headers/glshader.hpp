#ifndef gl_shader_h
#define gl_shader_h
#pragma once

#include <glad/glad.h>

#include <string>

class GLShader {
private:
  GLuint mId;

public:
  GLShader(const GLchar *vertexShaderSrcFilePath,
           const GLchar *fragmentShaderSrcFilePath, int* success);

  void use();

  bool setBool(const std::string &name, bool value) const;
  bool setInt(const std::string &name, GLint value) const;
  bool setFloat(const std::string &name, GLfloat value) const;
  bool setFloat(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const;
  bool setFloat(const std::string &name, GLfloat x, GLfloat y, GLfloat z,
                GLfloat w) const;
  bool setMatrix4(const std::string &name, GLsizei size, GLboolean transpose, const GLfloat* mat4ValuePtr) const;
};

#endif /* gl_shader_h */
