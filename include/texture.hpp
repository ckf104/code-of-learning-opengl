#ifndef __TEXTURE_HPP
#define __TEXTURE_HPP

#include <glad/glad.h>

#include <iostream>
#include <string>

class Shader;

class Texture2D {
 public:
  uint32_t pos;  // which texture unit
  uint32_t id;
  std::string name;

  Texture2D(const std::string& name_, uint32_t pos_, uint32_t format,
            uint32_t height, uint32_t width, const char* data,
            uint32_t Wrap_S = GL_REPEAT, uint32_t Wrap_T = GL_REPEAT,
            uint32_t sample_min = GL_LINEAR, uint32_t sample_max = GL_LINEAR);
  void Destroy();
  void setPos(Shader* program);
  void Activate() const;
};

#endif