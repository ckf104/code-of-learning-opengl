#include "game_object.hpp"
#include "resource_manager.hpp"

#include <cstdlib>
#include <algorithm>

GameObject::GameObject()
    : Position(0.0f, 0.0f),
      Size(1.0f, 1.0f),
      Color(1.0f),
      Rotation(0.0f),
      Sprite() {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D* sprite,
                       glm::vec4 color)
    : Position(pos), Size(size), Color(color), Rotation(0.0f), Sprite(sprite) {}

void GameObject::Draw(SpriteRenderer* renderer) {
  renderer->DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation,
                       this->Color);
}

void GameObject::Reset(glm::vec2 pos) { Position = pos; }

void GameObject::SetTexture2D(Texture2D* sprite) { Sprite = sprite; }

void GameObject::updatePosSize(glm::vec2 pos, glm::vec2 size) {
  Position = pos;
  Size = size;
}

Brick::Brick(uint32_t y, uint32_t x, bool isSolid, glm::vec2 pos,
             glm::vec2 size, Texture2D* sprite,
             glm::vec4 color /*=glm::vec3(1.0f)*/)
    : GameObject(pos, size, sprite, color) {
  this->y = y;
  this->x = x;
  this->IsSolid = isSolid;
  this->Destroyed = false;
}
void Brick::Draw(SpriteRenderer* renderer) {
  if (!this->Destroyed) this->GameObject::Draw(renderer);
}

void Brick::Reset(glm::vec2 pos) {
  Destroyed = false;
  GameObject::Reset(pos);
}

void Brick::updateWH(float uintW, float uintH) {
  updatePosSize(glm::vec2(x * uintW, y * uintH), glm::vec2(uintW, uintH));
}

MoveObject::MoveObject(float velocity, glm::vec2 pos, glm::vec2 size,
                       Texture2D* sprite, glm::vec4 color)
    : GameObject(pos, size, sprite, color) {
  Velocity = velocity;
}

BallObject::BallObject() : GameObject(), Radius(12.5f), Stuck(true) {}

BallObject::BallObject(glm::vec2 velocity, glm::vec2 pos, float radius,
                       Texture2D* sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite,
                 glm::vec4(1.0f)),
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
void BallObject::SetVelocity(glm::vec2 velocity) { Velocity = velocity; }

// resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
  Position = position;
  Velocity = velocity;
  Stuck = true;
}
void BallObject::CancelStuck() { Stuck = false; }

void BallObject::Reset(glm::vec2 pos) {
  InitVelocity();
  Stuck = true;
  GameObject::Reset(pos);
}

void BallObject::InitVelocity(float random, int signx) {
  if (random == -1.0f) random = (rand() % 30) + 30;
  float vx = ResourceManager::ball_velocity * glm::sin(glm::radians(random));
  if ((int)random % 2 == 0) vx = -vx;
  if (signx != 0) vx = signx * glm::abs(vx);
  float vy = -ResourceManager::ball_velocity * glm::cos(glm::radians(random));
  SetVelocity(glm::vec2(vx, vy));
}

CollisionDir BallObject::CheckCollision(GameObject* obj, bool penetration,
                                        float* hitx)  // AABB - Circle collision
{
  // get center point circle first
  glm::vec2 center(Position + Radius);
  // calculate AABB info (center, half-extents)
  glm::vec2 aabb_half_extents(obj->Size.x / 2.0f, obj->Size.y / 2.0f);
  glm::vec2 aabb_center(obj->Position + aabb_half_extents);
  // get difference vector between both centers
  glm::vec2 difference = center - aabb_center;
  glm::vec2 clamped =
      glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
  // add clamped value to AABB_center and we get the value of box closest to
  // circle
  glm::vec2 closest = aabb_center + clamped;
  // retrieve vector between center circle and closest point AABB and check if
  // length <= radius
  difference = closest - center;
  if (glm::length(difference) >= Radius) return CollisionDir::no;

  glm::vec2 dirs[] = {
      glm::vec2(0.0f, 1.0f),   // up
      glm::vec2(1.0f, 0.0f),   // right
      glm::vec2(0.0f, -1.0f),  // down
      glm::vec2(-1.0f, 0.0f)   // left
  };
  glm::vec2 norm_diff = glm::normalize(difference);
  glm::vec2* iter = std::max_element(
      dirs, dirs + 4, [&norm_diff](const glm::vec2& a, const glm::vec2& b) {
        return glm::dot(norm_diff, a) < glm::dot(norm_diff, b);
      });

  if (penetration) {
    glm::vec2 pene_vec = (norm_diff * Radius) - difference;
    Position = Position - pene_vec;
  }
  if (hitx) *hitx = Position.x + Radius;

  if (iter == dirs || iter == dirs + 2) {
    Velocity.y = -Velocity.y;
    return CollisionDir::y;
  }
  Velocity.x = -Velocity.x;
  return CollisionDir::x;
}