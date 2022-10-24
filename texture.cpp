#include "texture.hpp"
#include "hello.hpp"
#include "shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

Texture::Texture(uint32_t pos_, const char* image_path, const char* name_, uint32_t format)
    : name(name_) {
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(image_path, &width, &height, &nrChannels, 0);
  if (!data) {
    std::cout << "[error] Failed to load image " << image_path << std::endl;
    std::terminate();
  }
  pos = pos_;

  // glActiveTexture(GL_TEXTURE0 + pos);
  glGenTextures(1, &buf_id);
  glBindTexture(GL_TEXTURE_2D, buf_id);

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glGenerateMipmap(GL_TEXTURE_2D);
  checkErr(); 

  stbi_image_free(data);
}

void Texture::texActive() {
  glActiveTexture(GL_TEXTURE0 + pos);
  glBindTexture(
      GL_TEXTURE_2D,
      buf_id);  // current 2D texture is buf_id and as texture unit pos.
}

void Texture::texDestroy() { glDeleteTextures(1, &buf_id); }

void Texture::setPos(Shader* program) {
  program->use(); // active program before using glUniform1i
  glUniform1i(glGetUniformLocation(program->getid(), name.c_str()), pos);
  checkErr();
}