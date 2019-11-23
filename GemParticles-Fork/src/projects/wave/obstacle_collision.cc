#include "projects/wave/obstacle_collision.hh"

namespace gem { namespace particle {
namespace wave_project {

ObstacleCollision::ObstacleCollision(
  const glm::f32vec3 &a_fvPositionOfInterest)
 { m_fvPositionOfInterest = a_fvPositionOfInterest;
};

void ObstacleCollision::Update(double a_dt, const std::shared_ptr<ParticlePool<particle::lit_particles_project::LitParticlesData> >& a_pPool) {
  // TODO: Deal with the delta double precision casted to float later
  // (GLM vec3 or vec4 doesn't support operations with doubles...)
  const float fDt = static_cast<float>(a_dt);

  // Using the euler model to update the positions and velocities
  glm::f32vec2 currentPOI = glm::f32vec2(m_fvPositionOfInterest.x, m_fvPositionOfInterest.z);
  for (std::size_t i = 0; i < a_pPool->GetActiveParticleCount(); ++i) {
    if (glm::distance(glm::f32vec2(a_pPool->pData->m_position[i].x, a_pPool->pData->m_position[i].z), currentPOI) < m_radius) {
		a_pPool->ReleaseLightIndex(i);
		a_pPool->Sleep(i);
	}
  }
}
} /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */