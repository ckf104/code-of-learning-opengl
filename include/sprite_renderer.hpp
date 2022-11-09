#ifndef __sprite_renderer_HPP
#define __sprite_renderer_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "shader.hpp"

#include <map>

class SpriteRenderer {
 public:
  // Constructor (inits shaders/shapes)
  SpriteRenderer(Shader* shader);
  // Destructor
  virtual ~SpriteRenderer();
  // Renders a defined quad textured with given sprite
  void DrawSprite(Texture2D* texture, glm::vec2 position,
                  glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                  glm::vec4 color = glm::vec4(1.0f));
  void SimpleDrawSprite(Texture2D* texture, glm::vec2 position, glm::vec2 size);
  void SetProjection(const glm::mat4& pro);

  Shader* shader;

 private:
  // Render state
  uint32_t VAO, VBO;
};

class Texture2D;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
  Texture2D* tex;
  glm::ivec2 Size;     // size of glyph
  glm::ivec2 Bearing;  // offset from baseline to left/top of glyph
  uint32_t Advance;    // horizontal offset to advance to next glyph
};

// A renderer class for rendering text displayed by a font loaded using the
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer : public SpriteRenderer {
 public:
  // holds a list of pre-compiled Characters
  TextRenderer(Shader* shader, const std::string font,
                             uint32_t size);
  virtual ~TextRenderer();
  std::map<char, Character> Characters;
  // pre-compiles a list of characters from the given font
  void Load(const std::string& font, unsigned int fontSize);
  // renders a string of text using the precompiled list of characters
  void RenderText(const std::string& text, float x, float y, float scale,
                  glm::vec4 color = glm::vec4(1.0f));

 private:
};

#endif