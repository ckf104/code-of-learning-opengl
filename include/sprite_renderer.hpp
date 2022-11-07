#ifndef __sprite_renderer_HPP
#define __sprite_renderer_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "shader.hpp"

class SpriteRenderer {
 public:
  // Constructor (inits shaders/shapes)
  SpriteRenderer(Shader* shader);
  // Destructor
  ~SpriteRenderer();
  // Renders a defined quad textured with given sprite
  void DrawSprite(Texture2D* texture, glm::vec2 position,
                  glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                  glm::vec3 color = glm::vec3(1.0f));
  void SetProjection(const glm::mat4& pro);

 private:
  // Render state
  Shader* shader;
  uint32_t VAO, VBO;
};

#endif