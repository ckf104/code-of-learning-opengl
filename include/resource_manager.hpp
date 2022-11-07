#ifndef __resource_manager_HPP
#define __resource_manager_HPP

#include <map>
#include <string>
#include <unordered_set>

#include <glad/glad.h>

#include "texture.hpp"
#include "shader.hpp"
#include "hello.hpp"

class GameLevel;
class SpriteRenderer;

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager {
 public:
  // resource storage
  static glm::vec3 ObjColor[6];

  static const char* solid_block;
  static const char* solid_block_file;

  static const char* block;
  static const char* block_file;

  static const char* paddle;
  static const char* paddle_file;

  static const char* background;
  static const char* background_file;

  static const char* ball;
  static const char* ball_file;

  static const char* texname_in_shader;

  static const char* vshader_file;
  static const char* fshader_file;
  static const char* shader;

  static const float paddle_velocity;
  static const float ball_velocity;
  static constexpr float ball_radius = 12.5f;

  static constexpr glm::vec2 paddle_size = glm::vec2(100.0f, 20.0f);
  static constexpr glm::vec2 paddle_pos =
      glm::vec2(SCREEN_W / 2 - paddle_size.x / 2 , SCREEN_H - paddle_size.y);
  static constexpr glm::vec2 ball_pos = paddle_pos + glm::vec2(paddle_size.x / 2.0f - ball_radius, 
                                              -ball_radius * 2.0f);

  static std::map<std::string, SpriteRenderer*> Renderers;
  static std::map<std::string, Texture2D*> Textures;
  static std::unordered_set<GameLevel*> levels;

  // static std::unordered_set<SpriteRenderer*> renderers;

  static void LoadInitRes();
  // loads (and generates) a shader program from file loading vertex, fragment
  // (and geometry) shader's source code. If gShaderFile is not nullptr, it also
  // loads a geometry shader
  static SpriteRenderer* LoadRenderer(const char* vShaderFile,
                                      const char* fShaderFile,
                                      const char* gShaderFile,
                                      const std::string& name);
  // retrieves a stored sader
  static SpriteRenderer* GetRenderer(const std::string& name);
  // loads (and generates) a texture from file
  static Texture2D* LoadTexture(const char* file, const std::string& name,
                                const std::string& name_in_shader,
                                uint32_t texpos, bool reverse_y = true);
  // retrieves a stored texture
  static Texture2D* GetTexture(const std::string& name);

  static GameLevel* LoadGameLevel(uint32_t level, uint32_t levelW,
                                  uint32_t levelH);
  void RmGameLevel(GameLevel* glevel);
  // properly de-allocates all loaded resources
  static void Clear();

 private:
  // private constructor, that is we do not want any actual resource manager
  // objects. Its members and functions should be publicly available (static).
  ResourceManager() {}
  // loads and generates a shader from file
  static Shader* loadShaderFromFile(const char* vShaderFile,
                                    const char* fShaderFile,
                                    const char* gShaderFile = nullptr);
  // loads a single texture from file
  static Texture2D* loadTextureFromFile(const char* file,
                                        const std::string& name_in_shader,
                                        uint32_t texpos,
                                        bool revserse_y = true);
  static std::string levelToFile(uint32_t level);
};

#endif