#pragma once
#ifndef OBSTACLE_COLLISION
#define OBSTACLE_COLLISION

#include "dynamics/dynamic.hh"
#include "projects/lit_particles/lit_particles_pool.hh"
#include "projects/lit_particles/lit_particles_data.hh"
#include "core/particle_pool_core.hh"


namespace gem { namespace particle {
namespace wave_project {
class ObstacleCollision : public Dynamic<particle::lit_particles_project::LitParticlesData> {
public:
  ObstacleCollision(const glm::f32vec3 &a_fvPositionOfInterest);
  ~ObstacleCollision() = default;

  void SetPOI(const glm::f32vec3& a_vPOI) { m_fvPositionOfInterest = a_vPOI; }
  void Update(double a_dt, const std::shared_ptr<ParticlePool<particle::lit_particles_project::LitParticlesData> > &a_pPool) override;

private:
  glm::f32vec3	m_fvPositionOfInterest;
  float m_radius = 1.0f;
}; /* class ObstacleCollision*/
} /* namespace wave_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: OBSTACLE_COLLISION */
