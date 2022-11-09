#include "particle.hpp"

ParticleGenerator::ParticleGenerator(Texture2D *texture, unsigned int amount)
    : texture(texture),
      amount(amount),
      particles(nullptr),
      lastUsedParticle(0),
      particleSize(10.0f, 10.0f) {
  init();
}

ParticleGenerator::~ParticleGenerator() {
  if (particles) delete[] particles;
}

void ParticleGenerator::SetTexture(Texture2D *texture) {
  this->texture = texture;
}

void ParticleGenerator::Update(float dt, BallObject &object,
                               unsigned int newParticles, glm::vec2 offset) {
  // add new particles

  // update all particles
  for (unsigned int i = 0; i < this->amount; ++i) {
    Particle &p = this->particles[i];
    p.Life -= dt;         // reduce life
    if (p.Life > 0.0f) {  // particle is alive, thus update
#ifdef particleVelocity
      p.Position += p.Velocity * dt;
#endif
      p.Color.a -= dt * 2.5f;
    }
  }
  //std::cout << "dt: " << dt << std::endl;

  for (unsigned int i = 0; i < newParticles; ++i) {
    this->respawnParticle(this->particles[(lastUsedParticle + i) % amount],
                          object, offset);
  }
  lastUsedParticle = (lastUsedParticle + newParticles) %
                     amount;  // reverse up-down code order ?
}

// render all particles
void ParticleGenerator::Draw(SpriteRenderer *renderer) {
  // use additive blending to give it a 'glow' effect
  //int cnt = 0;
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  Particle *particle = particles;
  for (int i = 0; i < amount; ++i, particle++) {
    if (particle->Life > 0.0f) {
      //cnt++;
      renderer->DrawSprite(texture, particle->Position, particleSize,
                           particle->Rotation, particle->Color);
    }
  }
  //std::cout << "particle cnt: " << cnt << std::endl;
  // don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() { particles = new Particle[amount]; }

void ParticleGenerator::respawnParticle(Particle &particle, BallObject &object,
                                        glm::vec2 offset) {
  float random = ((rand() % 100) - 50) / 10.0f;
  float rColor = 0.5f + ((rand() % 100) / 100.0f);
  particle.Position = (object.Position + random + offset) + particleSize / 4.0f;
  particle.Rotation = rand() % 360;
  particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.Life = 1.0f;
#ifdef particleVelocity
  particle.Velocity = object.Velocity * 0.1f;
#endif
}