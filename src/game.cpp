#include "game.hpp"
#include "resource_manager.hpp"
#include "game_level.hpp"
#include "hello.hpp"

#include <algorithm>
#include <cstdlib>

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_ACTIVE),
      Keys(),
      Width(width),
      Height(height),
      background(glm::vec2(0, 0), glm::vec2(width, height)),
      paddle(ResourceManager::paddle_velocity, ResourceManager::paddle_pos,
             ResourceManager::paddle_size),
      ball(glm::vec2(0.0f), ResourceManager::ball_pos,
           ResourceManager::ball_radius) {}

Game::~Game() {}

void Game::Init() {
  now_level = 1;
  State = GameState::GAME_ACTIVE;
  glm::mat4 pro = glm::ortho(0.0f, static_cast<float>(Width),
                             static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
  ResourceManager::LoadInitRes();
  ResourceManager::LoadRenderer(ResourceManager::vshader_file,
                                ResourceManager::fshader_file, nullptr,
                                ResourceManager::shader)
      ->SetProjection(pro);
  cur = ResourceManager::LoadGameLevel(now_level, Width, Height / 2);
  background.SetTexture2D(
      ResourceManager::GetTexture(ResourceManager::background));
  paddle.SetTexture2D(ResourceManager::GetTexture(ResourceManager::paddle));
  ball.SetTexture2D(ResourceManager::GetTexture(ResourceManager::ball));

  srand((unsigned)time(nullptr));
  int random = (rand() % 30) + 30;
  float vx = ResourceManager::ball_velocity * glm::sin(glm::radians((float)random));
  if (random % 2 == 0) vx = -vx;
  float vy = -ResourceManager::ball_velocity * glm::cos(glm::radians((float)random));
  ball.SetVelocity(glm::vec2(vx, vy));
}

void Game::Update(float dt) {
   ball.Move(dt, Width);
}

void Game::ProcessInput(float dt, GLFWwindow* window) {
  glm::vec2 pos = paddle.Position;
  float v = dt * paddle.Velocity;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos.x -= v;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos.x += v;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) ball.CancelStuck();
  pos.x = std::clamp(pos.x, 0.0f, Width - paddle.Size.x);
  paddle.updatePosSize(pos, paddle.Size);
}

void Game::Render() {
  SpriteRenderer* sprite =
      ResourceManager::GetRenderer(ResourceManager::shader);
  background.Draw(sprite);
  paddle.Draw(sprite);
  cur->Draw(sprite);
  ball.Draw(sprite);
}

void Game::SetScreen(uint32_t width, uint32_t height) {
  Width = width, Height = height;
  cur->UpdateWH(Width, Height / 2);
  background.updatePosSize(glm::vec2(0, 0), glm::vec2(Width, Height));
  glm::mat4 pro = glm::ortho(0.0f, static_cast<float>(Width),
                             static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
  ResourceManager::GetRenderer(ResourceManager::shader)->SetProjection(pro);
  paddle.updatePosSize(glm::vec2(paddle.Position.x, Height - paddle.Size.y),
                       paddle.Size);
}