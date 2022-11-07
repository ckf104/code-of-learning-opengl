#ifndef __game_object_HPP
#define __game_object_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "sprite_renderer.hpp"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject {
 public:
  // object state
  glm::vec2 Position, Size;
  glm::vec3 Color;
  float Rotation;
  // render state
  Texture2D* Sprite;
  // constructor(s)
  GameObject();
  GameObject(glm::vec2 pos, glm::vec2 size, Texture2D* sprite = nullptr,
             glm::vec3 color = glm::vec3(1.0f));
  // draw sprite
  virtual void Draw(SpriteRenderer* renderer);
  void updatePosSize(glm::vec2 position, glm::vec2 size);
  void SetTexture2D(Texture2D* sprite);
};

class Brick : public GameObject {
 public:
  uint32_t x, y;
  bool IsSolid;
  bool Destroyed;

  Brick(uint32_t y, uint32_t x, bool isSolid, glm::vec2 pos, glm::vec2 size,
        Texture2D* sprite, glm::vec3 color = glm::vec3(1.0f));
  virtual void Draw(SpriteRenderer* renderer);
  void updateWH(float uintW, float uintH);
};

class MoveObject : public GameObject {
 public:
  float Velocity;
  MoveObject(float velocity, glm::vec2 pos, glm::vec2 size,
             Texture2D* sprite = nullptr, glm::vec3 color = glm::vec3(1.0f));
};

class BallObject : public GameObject {
 public:
  // ball state
  float Radius;
  bool Stuck;
  glm::vec2 Velocity;
  // constructor(s)
  BallObject();
  BallObject(glm::vec2 velocity, glm::vec2 pos, float radius,
             Texture2D* sprite = nullptr);
  // moves the ball, keeping it constrained within the window bounds (except
  // bottom edge); returns new position
  glm::vec2 Move(float dt, unsigned int window_width);
  void SetVelocity(glm::vec2 velocity);
  // resets the ball to original state with given position and velocity
  void Reset(glm::vec2 position, glm::vec2 velocity);
  void CancelStuck();
};

#endif