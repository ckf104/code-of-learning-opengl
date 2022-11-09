#ifndef __SHADER_HPP
#define __SHADER_HPP

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
  // state
  uint32_t ID;
  // constructor
  Shader();
  Shader(const char *vertexSource, const char *fragmentSource,
         const char *geometrySource = nullptr);
  // sets the current shader as active
  Shader &Use();
  // compiles the shader from given source code
  void Compile(const char *vertexSource, const char *fragmentSource,
               const char *geometrySource =
                   nullptr);  // note: geometry source code is optional
  // utility functions
  void SetBool(const char *name, bool value, bool useShader = false);
  void SetFloat(const char *name, float value, bool useShader = false);
  void SetInteger(const char *name, int value, bool useShader = false);
  void SetVector2f(const char *name, float x, float y, bool useShader = false);
  void SetVector2f(const char *name, const glm::vec2 &value,
                   bool useShader = false);
  void SetVector3f(const char *name, float x, float y, float z,
                   bool useShader = false);
  void SetVector3f(const char *name, const glm::vec3 &value,
                   bool useShader = false);
  void SetVector4f(const char *name, float x, float y, float z, float w,
                   bool useShader = false);
  void SetVector4f(const char *name, const glm::vec4 &value,
                   bool useShader = false);
  void SetMatrix4(const char *name, const glm::mat4 &matrix,
                  bool useShader = false);
  void Destroy();

 private:
  // checks if compilation or linking failed and if so, print the error logs
  void checkCompileErrors(unsigned int object, std::string type);
};

#endif