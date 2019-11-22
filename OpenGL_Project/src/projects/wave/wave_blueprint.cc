#include "projects/wave/wave_blueprint.hh"
//C system files
//C++ system files
#include <memory>
#include <limits>
//Other libraries' .h files
//Your project's .h files
#include "core/particle_module.hh"

//Project specific components
#include "projects/lit_particles/lit_particles_system.hh"
#include "projects/lit_particles/lit_particles_renderer.hh"
#include "projects/lit_particles/lit_particles_updater.hh"
#include "projects/lit_particles/ground_collision.hh"
#include "projects/lit_particles/lit_particles_gravity_acceleration.hh"
#include "projects/wave/obstacle_collision.hh"
#include "projects/wave/wave_emitter.hh"
#include "emitters/spherical_stream_emitter.hh"
#include "renderers/core_opengl_renderer.hh"

namespace gem { namespace particle {
namespace wave_project {
namespace blueprint { namespace wave_system_builder {
namespace {
const glm::f32vec3 _ZeroVector = glm::f32vec3(0.0f, 0.0f, 0.0f);
// Instead of using setters for every attribute, might as well put them public.
// These parameters will be used during the Create() function to properly build the particle system
//glm::u8vec4   _ParticleColor    = { 255u, 0u, 255u, 255u };
glm::u8vec4   _ParticleColor    = { 66u, 245u, 236u, 255u };
glm::f32vec3  _POI              = { 0.0f, 0.0f, 0.0f };
glm::f32vec3  _InitialVelocity  = { 0.0f, 0.0f, 3.0f };
std::size_t   _ParticleCount    = 251u;
std::size_t   _LightsCount      = 700u;
std::string   _TexturePath;
std::string   _ParticleSystemName;


// Handles on the dynamics to hand them over to the event handler
// There are only used during the construction of the particle system
std::shared_ptr<ObstacleCollision> _ObstacleCollisionHandle;
}

void Create() {
  _ObstacleCollisionHandle = std::make_shared<ObstacleCollision>(_POI);
  auto wParticleSystem = std::make_unique<LitParticleSystem>(_ParticleCount, _LightsCount, _ParticleSystemName);
  wParticleSystem->BindRenderer(std::make_unique<particle::lit_particles_project::LitParticlesRenderer>(_TexturePath,0.15f));
  // wParticleSystem->AddDynamic(_ObstacleCollisionHandle);
  wParticleSystem->AddDynamic(std::make_unique<particle::lit_particles_project::LitParticleUpdater>());
  wParticleSystem->AddDynamic(std::make_unique<particle::lit_particles_project::GroundCollision>());
  wParticleSystem->AddDynamic(std::make_unique<particle::lit_particles_project::GravityAcceleration>());
  
  wParticleSystem->AddEmitter(std::make_unique<WaveEmitter>(_POI, _InitialVelocity, 10.0f, 300.0, 0.2f, _ParticleColor));
  
  //wParticleSystem->AddEmitter(std::make_unique<RandomFountainEmitter>(_POI, 1.5f, 600.0, 4.0f, _ParticleColor));
  particle_module::AddSystem(std::move(wParticleSystem));
 
}

std::shared_ptr<ObstacleCollision> GetObstacleHandle() { return _ObstacleCollisionHandle; }


void SetTexture(const std::string& a_sTexturePath) { _TexturePath = a_sTexturePath; }
void SetParticleColor(const glm::vec4 &color)       { _ParticleColor = color;     }
void SetPOI(const glm::f32vec3 &pos)                { _POI = pos;                 }
void SetParticleCount(std::size_t count)            { _ParticleCount = count;     }
void SetParticleSystemName(const std::string &name) { _ParticleSystemName = name; }
} /* namespace wave_system_builder */
} /* namespace blueprint */
} /* namespace wave_project */
} /* namespace particle */
} /* namespace gem */
