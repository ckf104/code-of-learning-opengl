#ifndef __game_level_HPP
#define __game_level_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.hpp"
#include "sprite_renderer.hpp"
#include "resource_manager.hpp"

/// GameLevel holds all Tiles as part of a Breakout level and
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel {
 public:
  // level state
  std::vector<Brick*> Bricks;
  uint32_t brickW, brickH;
  // constructor
  GameLevel(const std::vector<std::vector<uint32_t>>& tile, uint32_t levelW,
            uint32_t levelH);
  ~GameLevel();
  // loads level from file
  void Load(const char* file, unsigned int levelWidth,
            unsigned int levelHeight);
  // render level
  void Draw(SpriteRenderer* renderer);
  // check if the level is completed (all non-solid tiles are destroyed)
  bool IsCompleted();
  void UpdateWH(uint32_t levelW, uint32_t levelH);
};

#endif