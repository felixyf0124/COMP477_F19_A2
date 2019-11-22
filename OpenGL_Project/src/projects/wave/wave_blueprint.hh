#pragma once
#ifndef WAVE_BLUEPRINT_HH
#define WAVE_BLUEPRINT_HH

//C system files
//C++ system files
#include <string>
#include <memory>
//Other libraries' .h files
#include "glm/glm.hpp"
//Your project's .h files
#include "projects/wave/obstacle_collision.hh"


namespace gem { namespace particle {
namespace wave_project {
namespace blueprint { namespace wave_system_builder {
// This creates the said blueprint and adds it to the ParticleModule
void Create();
// Some setters for the parameters used for the creation of the system

std::shared_ptr<ObstacleCollision> GetObstacleHandle();

void SetParticleColor(const glm::vec4 &color);
void SetPOI(const glm::f32vec3 &pos);
void SetParticleCount(std::size_t count);
void SetTexture(const std::string& a_sTexturePath);
void SetParticleSystemName(const std::string &name);
} /* namespace wave_system_builder */
} /* namespace blueprint */
} /* namespace WAVE_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: WAVE_BLUEPRINT_HH */