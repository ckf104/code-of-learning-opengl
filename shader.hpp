#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
 public:
  // 程序ID
  uint32_t programid;

  // 构造器读取并构建着色器
  Shader(const char* vertexPath, const char* fragmentPath);
  // 使用/激活程序
  void use();
  // uniform工具函数
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setFVec3(const std::string& name, const glm::vec3& value) const;
  void setFMat4(const std::string& name, const glm::mat4& value) const;
  void setFMat3(const std::string& name, const glm::mat3& value) const;
  uint32_t getid();
  void destroy();
};

#endif