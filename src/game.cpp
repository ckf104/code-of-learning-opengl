#include "game.hpp"
#include "resource_manager.hpp"
#include "game_level.hpp"
#include "hello.hpp"

#include <algorithm>
#include <cstdlib>

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_INIT),
      Width(width),
      Height(height),
      background(glm::vec2(0, 0), glm::vec2(width, height)),
      paddle(ResourceManager::paddle_velocity, ResourceManager::paddle_pos,
             ResourceManager::paddle_size),
      ball(glm::vec2(0.0f), ResourceManager::ball_pos,
           ResourceManager::ball_radius),
      particleGen(nullptr, 350) {
  key_s = key_w = false;
  shake_time = 0.0f;
}

Game::~Game() {}

void Game::Init() {
  now_level = 1;
  glm::mat4 pro = glm::ortho(0.0f, static_cast<float>(Width),
                             static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
  ResourceManager::LoadInitRes();
  ResourceManager::LoadRenderer(ResourceManager::vshader_file,
                                ResourceManager::fshader_file, nullptr,
                                ResourceManager::shader)
      ->SetProjection(pro);
  textRenderer = ResourceManager::LoadTextRenderer(
      ResourceManager::vshader_file, ResourceManager::textfshader_file, nullptr,
      ResourceManager::text_shader);
  textRenderer->SetProjection(pro);

  cur = ResourceManager::LoadGameLevel(now_level, Width, Height / 2);
  background.SetTexture2D(
      ResourceManager::GetTexture(ResourceManager::background));
  paddle.SetTexture2D(ResourceManager::GetTexture(ResourceManager::paddle));
  ball.SetTexture2D(ResourceManager::GetTexture(ResourceManager::ball));

  srand((unsigned)time(nullptr));
  ball.InitVelocity();

  particleGen.SetTexture(
      ResourceManager::GetTexture(ResourceManager::particle));

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::Update(float dt, GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (shake_time > 0.0f) {
    shake_time -= dt;
    if (shake_time <= 0.0f) {
      shake_time = 0.0f;
      ResourceManager::GetRenderer(ResourceManager::shader)
          ->shader->SetBool("shake", false, true);
    }
  }

  ProcessInput(dt, window);
  ball.Move(dt, Width);
  DoCollisions();
  particleGen.Update(dt, ball, 5, glm::vec2(ball.Radius / 2.0f));
}

void Game::DrawText(const std::string& text, float size, glm::vec2 pos,
                    glm::vec4 color) {
  textRenderer->RenderText(text, pos.x, pos.y, size, color);
}

void Game::DoCollisions() {
  float delta = 1.0f, eplison = 0.001f;
  CollisionDir dir;
  for (Brick* brick : cur->Bricks) {
    if (!brick->Destroyed) {
      dir = ball.CheckCollision(brick, true);
      if (dir != CollisionDir::no && !brick->IsSolid) {
        brick->Destroyed = true;
        cur->destroyed_brk++;
        if (cur->IsCompleted()) State = GameState::GAME_WIN;
      }
      if (dir != CollisionDir::no) {
        if (State == GameState::GAME_ACTIVE) {
          SpriteRenderer* r =
              ResourceManager::GetRenderer(ResourceManager::shader);
          r->shader->SetBool("shake", true, true);
          shake_time = 0.3f;
        }
        return;
      }
    }
  }

  float hitx, percentage;
  dir = ball.CheckCollision(&paddle, false,
                            &hitx);  // hack for sticky paddle problem
  if (dir == CollisionDir::x) {
    if (ball.Position.x > paddle.Position.x) {
      ball.Position.x = paddle.Position.x + paddle.Size.x + delta;
      ball.Velocity.x = glm::abs(ball.Velocity.x);
    } else {
      ball.Position.x = paddle.Position.x - ball.Radius * 2.0f - delta;
      ball.Velocity.x = -glm::abs(ball.Velocity.x);
    }
  } else if (dir == CollisionDir::y) {
    ball.Velocity.y = -glm::abs(ball.Velocity.y);
    percentage = glm::abs(hitx - (paddle.Position.x + paddle.Size.x / 2)) /
                 (paddle.Size.x / 2);
    percentage = std::clamp(percentage, 0.12f, 0.95f) * 70;
    ball.InitVelocity(percentage, ball.Velocity.x < 0 ? -1 : 1);
  }

  if (ball.Position.y >= Height || ball.Position.x < 0 - eplison ||
      ball.Position.x + ball.Size.x > Width + eplison)
    Reset();
}

void Game::Reset() {
  State = GameState::GAME_INIT;
  ball.Reset(ResourceManager::ball_pos);
  paddle.Reset(ResourceManager::paddle_pos);
  cur->Reset();
}

void Game::UpdateLevel(uint32_t level) {
  if (level >= 5)
    level = 1;
  else if (level <= 0)
    level = 4;
  ResourceManager::RmGameLevel(cur);
  cur = ResourceManager::LoadGameLevel(level, Width, Height / 2);
  now_level = level;
}

void Game::ProcessInput(float dt, GLFWwindow* window) {
  if (State == GameState::GAME_INIT) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      State = GameState::GAME_ACTIVE;
      ball.CancelStuck();
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      key_w = true;
    } else if (key_w) {
      key_w = false;
      UpdateLevel(now_level + 1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      key_s = true;
    } else if (key_s) {
      key_s = false;
      UpdateLevel(now_level - 1);
    }

  } else if (State == GameState::GAME_WIN) {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
      Reset();
    ball.Stuck = true;
  } else {
    glm::vec2 pos = paddle.Position;
    float v = dt * paddle.Velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos.x -= v;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos.x += v;
    pos.x = std::clamp(pos.x, 0.0f, Width - paddle.Size.x);
    paddle.updatePosSize(pos, paddle.Size);
  }
}

void Game::Render() {
  SpriteRenderer* sprite =
      ResourceManager::GetRenderer(ResourceManager::shader);
  if (shake_time > 0.0f)
    sprite->shader->SetFloat("time", (float)glfwGetTime(), true);
  
  background.Draw(sprite);
  paddle.Draw(sprite);
  cur->Draw(sprite);
  particleGen.Draw(sprite);
  ball.Draw(sprite);

  if (State == GameState::GAME_INIT) {
    DrawText("Press SPACE to start", 1.0f,
             glm::vec2(Width / 2.0f - 165.0f, Height / 2.0f));
    DrawText("Press W or S to select level", 0.75f,
             glm::vec2(Width / 2.0f - 180.0f, Height / 2.0f + 35.0f));
  } else if (State == GameState::GAME_WIN) {
    DrawText("You WON!!!", 1.0f, glm::vec2(320.0, Height / 2 - 20.0),
             glm::vec4(0.0, 1.0, 0.0, 1.0f));
    DrawText("Press ENTER to retry or ESC to quit", 1.0f,
             glm::vec2(130.0, Height / 2), glm::vec4(1.0, 1.0, 0.0, 1.0f));
  }
}

void Game::SetScreen(uint32_t width, uint32_t height) {
  glm::vec2 scale = glm::vec2((float)width / Width, (float)height / Height);

  Width = width, Height = height;
  cur->UpdateWH(Width, Height / 2);
  background.updatePosSize(glm::vec2(0, 0), glm::vec2(Width, Height));
  glm::mat4 pro = glm::ortho(0.0f, static_cast<float>(Width),
                             static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
  ResourceManager::GetRenderer(ResourceManager::shader)->SetProjection(pro);
  paddle.updatePosSize(
      glm::vec2(Width / 2 - paddle.Size.x / 2, Height - paddle.Size.y),
      paddle.Size);
  ball.updatePosSize(ball.Position * scale, ball.Size);
}