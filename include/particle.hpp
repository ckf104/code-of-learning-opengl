#ifndef __particle_HPP
#define __particle_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include "game_object.hpp"

class SpriteRenderer;

// Represents a single particle and its state
struct Particle {
#ifdef particleVelocity
  glm::vec2 Velocity;
#endif
  glm::vec2 Position;
  glm::vec4 Color;
  float Rotation;
  float Life;

  Particle()
      : Rotation(0.0f),
        Position(0.0f),
#ifdef particleVelocity
        Velocity(0.0f),
#endif
        Color(1.0f),
        Life(0.0f) {}
};

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator {
 public:
  // constructor
  ParticleGenerator(Texture2D* texture, unsigned int amount);
  // update all particles
  void Update(float dt, BallObject& object, unsigned int newParticles,
              glm::vec2 offset = glm::vec2(0.0f, 0.0f));
  // render all particles
  void Draw(SpriteRenderer* renderer);
  void SetTexture(Texture2D* texture);
  ~ParticleGenerator();

 private:
  // state
  Particle* particles;
  uint32_t amount;
  uint32_t lastUsedParticle;
  Texture2D* texture;
  glm::vec2 particleSize;
  // initializes buffer and vertex attributes
  void init();
  // returns the first Particle index that's currently unused e.g. Life <= 0.0f
  // or 0 if no particle is currently inactive
  unsigned int firstUnusedParticle();
  // respawns particle
  void respawnParticle(Particle& particle, BallObject& object,
                       glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif