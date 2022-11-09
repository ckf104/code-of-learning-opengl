#include "game_level.hpp"

#include <fstream>
#include <sstream>

void GameLevel::Draw(SpriteRenderer* renderer) {
  for (Brick* tile : Bricks) tile->Draw(renderer);
}

bool GameLevel::IsCompleted() {
  return num_brk == destroyed_brk;
}

GameLevel::GameLevel(const std::vector<std::vector<unsigned int> >& tileData,
                     unsigned int levelWidth, unsigned int levelHeight) {
  // calculate dimensions
  brickH = tileData.size();
  brickW = tileData[0].size();  
  num_brk = destroyed_brk = 0;

  float unit_width = levelWidth / static_cast<float>(brickW),
        unit_height = levelHeight / static_cast<float>(brickH);
  // initialize level tiles based on tileData
  for (unsigned int y = 0; y < brickH; ++y) {
    for (unsigned int x = 0; x < brickW; ++x) {
      // check block type from level data (2D level array)
      glm::vec2 pos(unit_width * x, unit_height * y);
      glm::vec2 size(unit_width, unit_height);
      glm::vec4 color = glm::vec4(ResourceManager::ObjColor[tileData[y][x]], 1.0f);
      bool isSolid = tileData[y][x] == 1;
      const char* texName = tileData[y][x] == 1 ? ResourceManager::solid_block
                                                : ResourceManager::block;
      Texture2D* sprite = ResourceManager::GetTexture(texName);
      Brick* obj = new Brick(y, x, isSolid, pos, size, sprite, color);
      Bricks.push_back(obj);
      num_brk += !isSolid;
    }
  }
}

GameLevel::~GameLevel() {
  for (auto iter : Bricks) {
    delete iter;
  }
}

void GameLevel::Reset(){
  for(Brick* brick : Bricks){
    brick->Reset(brick->Position);
  }  
}

void GameLevel::UpdateWH(uint32_t levelW, uint32_t levelH) {
  float unit_width = levelW / static_cast<float>(brickW),
        unit_height = levelH / static_cast<float>(brickH);
  for (Brick* brick : Bricks) brick->updateWH(unit_width, unit_height);
}