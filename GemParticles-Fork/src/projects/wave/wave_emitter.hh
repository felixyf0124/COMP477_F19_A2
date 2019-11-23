#pragma once
#ifndef WAVE_EMITTER_HH
#define WAVE_EMITTER_HH

#include "emitters/emitter.hh"
#include "projects/lit_particles/lit_particles_pool.hh"

namespace gem { namespace particle {
namespace wave_project {
class WaveEmitter : public Emitter<particle::lit_particles_project::LitParticlesData> {
private:
  static const glm::vec4 LIGHT_COLOR;
public:
  WaveEmitter();
  WaveEmitter(
    const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime,
    double a_dEmissionRate,
    float a_dVelocityRandomization = 0.0f,
    const glm::u8vec4& a_color = DEFAULT_COLOR);
	virtual ~WaveEmitter() = default;

private:
  void Init(double a_dt, const std::shared_ptr<ParticlePool<particle::lit_particles_project::LitParticlesData> >& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) override;
  glm::f32vec3 RandomVelocity() const;

  float m_lineLength = 20.0f;
  float       m_fVelocityRandomization;
  glm::u8vec4 m_color;
}; /* class WaveEmitter*/
} /* namespace wave_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: WAVE_EMITTER_HH */