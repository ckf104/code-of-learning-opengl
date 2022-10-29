#ifndef __TEXTURE_HPP
#define __TEXTURE_HPP

#include <glad/glad.h>

#include <iostream>
#include <string>

class Shader;

class Texture {
 public:
  uint32_t pos;  // which texture unit
  uint32_t buf_id;
  std::string name;

  Texture(uint32_t pos_, const char* image_path, const char* name_, uint32_t repeat_way);
  void texActive();
  void texDestroy();
  void setPos(Shader* program);
};

#endif