#include "sprite_renderer.hpp"

SpriteRenderer::~SpriteRenderer() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  shader->Destroy();
  delete shader;
}

void SpriteRenderer::SetProjection(const glm::mat4& pro) {
  shader->SetMatrix4("projection", pro, true);
}

void SpriteRenderer::DrawSprite(Texture2D* texture, glm::vec2 position,
                                glm::vec2 size, float rotate, glm::vec3 color) {
  // prepare transformations
  shader->Use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));

  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
  model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
  model =
      glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  model = glm::scale(model, glm::vec3(size, 1.0f));  // last scale

  shader->SetMatrix4("model", model);

  // render textured quad
  shader->SetVector3f("spriteColor", color);

  texture->Activate();

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

SpriteRenderer::SpriteRenderer(Shader* shader) {
  this->shader = shader;
  // configure VAO/VBO
  // clang-format off
  float vertices[] = {
      // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
  };
  // clang-format on

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
