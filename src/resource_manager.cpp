#include "resource_manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.hpp"
#include "game_level.hpp"
#include "hello.hpp"

// Instantiate static variables
std::map<std::string, Texture2D *> ResourceManager::Textures;
std::map<std::string, SpriteRenderer *> ResourceManager::Renderers;
std::unordered_set<GameLevel *> ResourceManager::levels;

const char *ResourceManager::solid_block = "solid_block";
const char *ResourceManager::solid_block_file = "resources/solid_block.png";

const char *ResourceManager::block = "block";
const char *ResourceManager::block_file = "resources/block.png";

const char *ResourceManager::paddle = "paddle";
const char *ResourceManager::paddle_file = "resources/paddle.png";

const char *ResourceManager::background = "background";
const char *ResourceManager::background_file = "resources/background.jpg";

const char *ResourceManager::ball = "ball";
const char *ResourceManager::ball_file = "resources/face.png";

const char *ResourceManager::texname_in_shader = "image";

const float ResourceManager::paddle_velocity = 200.0f;
const float ResourceManager::ball_velocity = 200.0f;

const char *ResourceManager::vshader_file = "shader/vshader.glsl";
const char *ResourceManager::fshader_file = "shader/fshader.glsl";
const char *ResourceManager::shader = "shader";

// clang-format off
glm::vec3 ResourceManager::ObjColor[] = {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(0.8f, 0.8f, 0.7f),
    glm::vec3(0.2f, 0.6f, 1.0f),
    glm::vec3(0.0f, 0.7f, 0.0f),
    glm::vec3(0.8f, 0.8f, 0.4f),
    glm::vec3(1.0f, 0.5f, 0.0f),
};
// clang-format on

void ResourceManager::LoadInitRes() {
  LoadTexture(solid_block_file, solid_block, texname_in_shader, 0);
  LoadTexture(block_file, block, texname_in_shader, 0);
  LoadTexture(background_file, background, texname_in_shader, 0);
  LoadTexture(paddle_file, paddle, texname_in_shader, 0);
  LoadTexture(ball_file, ball, texname_in_shader, 0);
}

std::string ResourceManager::levelToFile(uint32_t level) {
  return "resources/level" + std::to_string(level) + ".txt";
}

GameLevel *ResourceManager::LoadGameLevel(uint32_t level, uint32_t levelW,
                                          uint32_t levelH) {
  // load from file
  uint32_t tileCode;
  std::string line, levelfile = levelToFile(level);
  std::ifstream fstream(levelfile);
  std::vector<std::vector<unsigned int>> tileData;
  GameLevel *glevel = nullptr;
  if (fstream) {
    while (std::getline(fstream, line))  // read each line from level file
    {
      std::istringstream sstream(line);
      std::vector<unsigned int> row;
      while (sstream >> tileCode)  // read each word separated by spaces
        row.push_back(tileCode);
      tileData.push_back(std::move(row));
    }
    if (tileData.size() > 0)
      glevel = new GameLevel(tileData, levelW, levelH);
    else
      std::cout << "ERROR::GAMELEVEL: file " << levelfile << " wrong format"
                << std::endl;
  } else {
    std::cout << "ERROR::GAMELEVEL: can't open file " << levelfile << std::endl;
  }
  if (glevel) levels.insert(glevel);
  return glevel;
}
void ResourceManager::RmGameLevel(GameLevel *glevel) { delete glevel; }

// TODO: do not need shader
SpriteRenderer *ResourceManager::LoadRenderer(const char *vShaderFile,
                                              const char *fShaderFile,
                                              const char *gShaderFile,
                                              const std::string &name) {
  Shader *s = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
  if (s) {
    SpriteRenderer *r = new SpriteRenderer(s);
    Renderers[name] = r;
    return r;
  }
  return nullptr;
}

SpriteRenderer *ResourceManager::GetRenderer(const std::string &name) {
  return Renderers[name];
}

Texture2D *ResourceManager::LoadTexture(const char *file,
                                        const std::string &name,
                                        const std::string &name_in_shader,
                                        uint32_t texpos,
                                        bool reverse_y /*=true*/) {
  Texture2D *t = loadTextureFromFile(file, name_in_shader, texpos, reverse_y);
  if (t) Textures[name] = t;
  return t;
}

Texture2D *ResourceManager::GetTexture(const std::string &name) {
  return Textures[name];
}

void ResourceManager::Clear() {
  // (properly) delete all shaders
  for (auto iter : Renderers) {
    delete iter.second;
  }
  Renderers.clear();
  // (properly) delete all textures
  for (auto iter : Textures) {
    iter.second->Destroy();
    delete iter.second;
  }
  Textures.clear();
  for (auto iter : levels) {
    delete iter;
  }
  levels.clear();
}

Shader *ResourceManager::loadShaderFromFile(const char *vShaderFile,
                                            const char *fShaderFile,
                                            const char *gShaderFile) {
  // 1. retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::string geometryCode;
  // open files
  std::ifstream vertexShaderFile(vShaderFile);
  std::ifstream fragmentShaderFile(fShaderFile);
  if (!vertexShaderFile || !fragmentShaderFile) {
    std::cout << "ERROR::SHADER: Failed to open vshader or fshader files"
              << std::endl;
    return nullptr;
  }

  std::stringstream vShaderStream, fShaderStream;

  // read file's buffer contents into streams
  vShaderStream << vertexShaderFile.rdbuf();
  fShaderStream << fragmentShaderFile.rdbuf();
  // convert stream into string
  vertexCode = vShaderStream.str();
  fragmentCode = fShaderStream.str();
  // if geometry shader path is present, also load a geometry shader
  if (gShaderFile != nullptr) {
    std::ifstream geometryShaderFile(gShaderFile);
    if (!geometryShaderFile) {
      std::cout << "ERROR::SHADER: Failed to open gshader files" << std::endl;
      return nullptr;
    }
    std::stringstream gShaderStream;
    gShaderStream << geometryShaderFile.rdbuf();
    geometryCode = gShaderStream.str();
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();
  const char *gShaderCode = nullptr;
  if (geometryCode.length() != 0)
    const char *gShaderCode = geometryCode.c_str();
  // 2. now create shader object from source code
  Shader *shader = new Shader(vShaderCode, fShaderCode, gShaderCode);
  return shader;
}

Texture2D *ResourceManager::loadTextureFromFile(
    const char *file, const std::string &name_in_shader, uint32_t texpos,
    bool reverse_y /*=true*/) {
  stbi_set_flip_vertically_on_load(
      !reverse_y);  // Orthogonal matrix has been reversed
  // load image
  uint32_t width, height, nrChannels;
  char *data = (char *)stbi_load(file, (int *)&width, (int *)&height,
                                 (int *)&nrChannels, 0);
  if (!data) {
    std::cout << "ERROR::TEXTURE: Failed to open texture files" << std::endl;
    return nullptr;
  }
  uint32_t format = nrChannels == 4 ? GL_RGBA : GL_RGB;
  Texture2D *texture =
      new Texture2D(name_in_shader, texpos, format, height, width, data);
  stbi_image_free(data);
  return texture;
}