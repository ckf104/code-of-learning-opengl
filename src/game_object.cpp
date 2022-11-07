#include "game_object.hpp"

GameObject::GameObject()
    : Position(0.0f, 0.0f),
      Size(1.0f, 1.0f),
      Color(1.0f),
      Rotation(0.0f),
      Sprite() {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D* sprite,
                       glm::vec3 color)
    : Position(pos), Size(size), Color(color), Rotation(0.0f), Sprite(sprite) {}

void GameObject::Draw(SpriteRenderer* renderer) {
  renderer->DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation,
                       this->Color);
}
void GameObject::SetTexture2D(Texture2D* sprite) { Sprite = sprite; }

void GameObject::updatePosSize(glm::vec2 pos, glm::vec2 size) {
  Position = pos;
  Size = size;
}

Brick::Brick(uint32_t y, uint32_t x, bool isSolid, glm::vec2 pos,
             glm::vec2 size, Texture2D* sprite,
             glm::vec3 color /*=glm::vec3(1.0f)*/)
    : GameObject(pos, size, sprite, color) {
  this->y = y;
  this->x = x;
  this->IsSolid = isSolid;
  this->Destroyed = false;
}
void Brick::Draw(SpriteRenderer* renderer) {
  if (!this->Destroyed) this->GameObject::Draw(renderer);
}
void Brick::updateWH(float uintW, float uintH) {
  updatePosSize(glm::vec2(x * uintW, y * uintH), glm::vec2(uintW, uintH));
}

MoveObject::MoveObject(float velocity, glm::vec2 pos, glm::vec2 size,
                       Texture2D* sprite, glm::vec3 color)
    : GameObject(pos, size, sprite, color) {
  Velocity = velocity;
}

BallObject::BallObject() : GameObject(), Radius(12.5f), Stuck(true) {}

BallObject::BallObject(glm::vec2 velocity, glm::vec2 pos, float radius,
                       Texture2D* sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite,
                 glm::vec3(1.0f)),
      Velocity(velocity),
      Radius(radius),
      Stuck(true) {}

glm::vec2 BallObject::Move(float dt, unsigned int window_width) {
  // if not stuck to player board
  if (!Stuck) {
    // move the ball
    Position += Velocity * dt;
    // then check if outside window bounds and if so, reverse velocity and
    // restore at correct position
    if (Position.x <= 0.0f) {
      Velocity.x = -Velocity.x;
      Position.x = 0.0f;
    } else if (Position.x + Size.x >= window_width) {
      Velocity.x = -Velocity.x;
      Position.x = window_width - Size.x;
    }
    if (Position.y <= 0.0f) {
      Velocity.y = -Velocity.y;
      Position.y = 0.0f;
    }
  }
  return Position;
}
void BallObject::SetVelocity(glm::vec2 velocity) {
  Velocity = velocity;
}

// resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
  Position = position;
  Velocity = velocity;
  Stuck = true;
}
void BallObject::CancelStuck() { Stuck = false; }