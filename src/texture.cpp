#include "texture.hpp"
#include "hello.hpp"
#include "shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

Texture2D::Texture2D(const std::string& name_, uint32_t pos_, uint32_t format,
                     uint32_t height, uint32_t width, const char* data,
                     uint32_t Wrap_S, uint32_t Wrap_T, uint32_t sample_min,
                     uint32_t sample_max) {
  // stbi_set_flip_vertically_on_load(true);
  pos = pos_;
  name = name_;

  // glActiveTexture(GL_TEXTURE0 + pos);
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sample_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sample_max);
  // glGenerateMipmap(GL_TEXTURE_2D);
  checkErr();
}

void Texture2D::Activate() const {
  glActiveTexture(GL_TEXTURE0 + pos);
  glBindTexture(GL_TEXTURE_2D,
                id);  // current 2D texture is buf_id and as texture unit pos.
}

void Texture2D::Destroy() { glDeleteTextures(1, &id); }

void Texture2D::setPos(Shader* program) {
  program->SetInteger(name.c_str(), pos, true);
  checkErr();
}