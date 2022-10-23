#include "shader.hpp"
#include "hello.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile(vertexPath);
  std::ifstream fShaderFile(fragmentPath);
  std::stringstream vShaderStream, fShaderStream;
  uint32_t vertex, fragment;
  const char* vShaderCode;
  const char* fShaderCode;

  if (!vShaderFile || !fShaderFile) {
    std::cout << "[error] wrong vertex or fragment shader file path"
              << std::endl;
    std::terminate();
  }

  // read file's buffer contents into streams
  vShaderStream << vShaderFile.rdbuf();
  fShaderStream << fShaderFile.rdbuf();
  // close file handlers
  vShaderFile.close();
  fShaderFile.close();
  // convert stream into string
  vertexCode = vShaderStream.str();
  fragmentCode = fShaderStream.str();
  vShaderCode = vertexCode.c_str();
  fShaderCode = fragmentCode.c_str();

  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  fragment = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  if (shaderlog("[error] vertex compile: ", vertex) != GL_TRUE)
    std::terminate();

  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  if (shaderlog("[error] fragment compile: ", fragment) != GL_TRUE)
    std::terminate();

  programid = glCreateProgram();
  glAttachShader(programid, vertex);
  glAttachShader(programid, fragment);
  glLinkProgram(programid);
  if (programlog("[error] program compile: ", programid) != GL_TRUE)
    std::terminate();

  // delete the shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(programid); }
void Shader::destroy() { glDeleteProgram(programid); }

void Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(programid, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(programid, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(programid, name.c_str()), value);
}