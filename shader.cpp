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
uint32_t Shader::getid() { return programid; }

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
void Shader::setFVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3f(glGetUniformLocation(programid, name.c_str()), value.x, value.y,
              value.z);
}
void Shader::setFMat4(const std::string& name, const glm::mat4& value) const {
  glUniformMatrix4fv(glGetUniformLocation(programid, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}
void Shader::setFMat3(const std::string& name, const glm::mat3& value) const {
  glUniformMatrix3fv(glGetUniformLocation(programid, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}
