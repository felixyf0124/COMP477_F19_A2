/*************************************************************************
 * Copyright (c) 2016 Fran�ois Trudel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
*************************************************************************/
#include "rain_particles_blueprint.hh"
//C system files
//C++ system files
#include <memory>
//Other libraries' .h files
//Your project's .h files
#include "core/particle_module.hh"
#include "core/particle_system.hh"
//#include "basic_stream_emitter.hh" // For debugging
#include "dynamics/gravity_acceleration.hh"
#include "dynamics/euler_particle_updater.hh"

//Project specific components
#include "projects/rain/rain_texture_renderer.hh"
#include "projects/rain/rain_emitter.hh"

namespace gem { namespace particle {
namespace rain_project {
namespace blueprint { namespace rain_particles_builder {
namespace {
  std::string _TexturePath;
  std::string _ParticleSystemName;
  double      _EmissionRate = 100000.0;
  std::size_t _ParticleCount = 1000000u;
}
void Create() {
  auto wParticleSystem = std::make_unique<ParticleSystem<> >(_ParticleCount, _ParticleSystemName);
  wParticleSystem->BindRenderer(std::make_unique<TextureCoreGLRenderer>(_TexturePath));
  wParticleSystem->AddDynamic(std::make_unique<EulerParticleUpdater>());
  wParticleSystem->AddDynamic(std::make_unique<GravityAcceleration>());
  wParticleSystem->AddEmitter(std::make_unique<RainEmitter>(10.0f, _EmissionRate));
  //wParticleSystem->AddEmitter(std::make_unique<BasicStreamEmitter>(glm::f32vec3(0.0f,0.0f,0.0f), glm::f32vec3(0.0f,0.0f,0.0f), 10000000.0,2.0)); // For debugging
  particle_module::AddSystem(std::move(wParticleSystem));
}

void SetTexture(const std::string& a_sTexturePath) { _TexturePath = a_sTexturePath; }
void SetParticleSystemName(const std::string& a_sSystemName) { _ParticleSystemName = a_sSystemName; }
void SetEmissionRate(double a_dEmissionRate) { _EmissionRate = a_dEmissionRate; }
void SetParticleCount(std::size_t a_unParticleCount) { _ParticleCount = a_unParticleCount; }
} /* namespace rain_particles_builder */
} /* namespace blueprint */
} /* namespace rain_project */
} /* namespace particle */
} /* namespace gem */

