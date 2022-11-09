#include "sprite_renderer.hpp"
#include "resource_manager.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

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
                                glm::vec2 size, float rotate, glm::vec4 color) {
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
  shader->SetVector4f("spriteColor", color);

  texture->Activate();

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void SpriteRenderer::SimpleDrawSprite(Texture2D* texture, glm::vec2 position,
                                      glm::vec2 size) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));
  model = glm::scale(model, glm::vec3(size, 1.0f));

  shader->SetMatrix4("model", model);
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

TextRenderer::TextRenderer(Shader* shader, const std::string font,
                           uint32_t size)
    : SpriteRenderer(shader) {
  Load(font, size);
}
TextRenderer::~TextRenderer() {}

void TextRenderer::Load(const std::string& font, unsigned int fontSize) {
  // first clear the previously loaded Characters
  this->Characters.clear();
  // then initialize and load the FreeType library
  FT_Library ft;
  if (FT_Init_FreeType(&ft))  // all functions return a value different than 0
                              // whenever an error occurred
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;
  // load font as face
  FT_Face face;
  if (FT_New_Face(ft, font.c_str(), 0, &face))
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
  // set size to load glyphs as
  FT_Set_Pixel_Sizes(face, 0, fontSize);

  // disable byte-alignment restriction
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  // then for the first 128 ASCII characters, pre-load/compile their characters
  // and store them
  for (GLubyte c = 0; c < 128; c++)  // lol see what I did there
  {
    // load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    Texture2D* t = ResourceManager::LoadTexture(
        (char*)face->glyph->bitmap.buffer, std::to_string(c),
        ResourceManager::texname_in_shader, 0, face->glyph->bitmap.width,
        face->glyph->bitmap.rows, GL_RED);

    // now store character for later use 
    Character character = {
        t, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x >> 6};
    /*std::cout << "int " << c << ", width " << face->glyph->bitmap.width
              << ", height " << face->glyph->bitmap.rows << ", advance "
              << (face->glyph->advance.x >> 6) << ", bearingY " << face->glyph->bitmap_top << std::endl;*/
    Characters.insert(std::pair<char, Character>(c, character));
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  // destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(const std::string& text, float x, float y, float scale,
                              glm::vec4 color) {
  // activate corresponding render state
  shader->Use();
  shader->SetVector4f("spriteColor", color);

  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character& ch = Characters[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y + (this->Characters['\\'].Bearing.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;
    SimpleDrawSprite(ch.tex, glm::vec2(xpos, ypos), glm::vec2(w, h));

    x += ch.Advance * scale;
    // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
