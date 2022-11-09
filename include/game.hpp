#ifndef __game_HPP
#define __game_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_object.hpp"
#include "particle.hpp"

class GameLevel;

// Represents the current state of the game
enum class GameState { GAME_INIT, GAME_ACTIVE, GAME_MENU, GAME_WIN };

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game {
 public:
  // game state
  GameState State;
  GameObject background;
  MoveObject paddle;
  BallObject ball;
  ParticleGenerator particleGen;
  GameLevel* cur;
  TextRenderer* textRenderer;
  uint32_t now_level;
  uint32_t Width, Height;
  float shake_time;
  bool key_w, key_s;
  // constructor/destructor
  Game(uint32_t width, uint32_t height);
  ~Game();
  // initialize game state (load all shaders/textures/levels)
  void Init();
  // game loop
  void Update(float dt, GLFWwindow* window);
  void Render();
  void SetScreen(uint32_t width, uint32_t height);

 private:
  void ProcessInput(float dt, GLFWwindow* window);
  void DoCollisions();
  void Reset();
  void DrawText(const std::string& text, float size, glm::vec2 pos,
                glm::vec4 color = glm::vec4(1.0f));
  void UpdateLevel(uint32_t level);
};

#endif