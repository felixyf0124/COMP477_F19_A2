#include "projects/wave/wave_emitter.hh"

#include <cstdlib>
#include <algorithm>
#include<random>
#include<cmath>
#include<chrono>
#include <vector>

#include "utils/light_module.hh"
#include "utils/imgui/imgui_property_editor.h"

namespace gem { namespace particle {
namespace wave_project {
std::vector<glm::f32vec3> GenerateLinearCoordinates(
	const glm::f32vec3& a_originCoordinate,
	float a_xlength,
	std::size_t a_unNumberOfCoords) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	std::uniform_real_distribution<float> uniform01(-1.0f, 1.0f);

	std::vector<glm::f32vec3> coords;
	coords.reserve(a_unNumberOfCoords);

	for (std::size_t i = 0ULL; i < a_unNumberOfCoords; i++) {
    float shift = a_xlength/2.0f * uniform01(generator);
 
    glm::f32vec3 coord;
    coord[0] = a_originCoordinate.x + shift;
    coord[1] = a_originCoordinate.y;
    coord[2] = a_originCoordinate.z;
    coords.push_back(std::move(coord));
  }
	return coords;
}

const glm::vec4 WaveEmitter::LIGHT_COLOR = { 0.0f, 0.0f, 0.5f, 0.5f };

float RandomFloat(float a_fMin, float a_fMax) {
  return a_fMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (a_fMax - a_fMin)));
}
WaveEmitter::WaveEmitter()
  : Emitter() { }

WaveEmitter::WaveEmitter(const glm::f32vec3& a_spawnLocation,
  const glm::f32vec3& a_spawnVelocity, float a_fLifetime, double a_dEmissionRate, 
  float a_fVelocityRandomization, const glm::u8vec4& a_color)
  : Emitter(a_spawnLocation, a_spawnVelocity, a_fLifetime, a_dEmissionRate),
    m_fVelocityRandomization(a_fVelocityRandomization), m_color(a_color) {
  // Add properties to the editor
  ImGuiPropertyEditor &editor = ImGuiPropertyEditor::GetInstance();
  editor.AddObject("Emitter", this);
  editor.AddProperty<PropertyType::VEC3>("Spawn velocity", &m_spawnVelocity, nullptr, 0.1f);
}

void WaveEmitter::Init(double a_dt, const std::shared_ptr<ParticlePool<particle::lit_particles_project::LitParticlesData> >& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {

	const std::size_t N = a_unEndID - a_unStartID;
	
	auto coords = GenerateLinearCoordinates(
    m_spawnLocation,
    m_lineLength, N);


  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pData->m_velocity[i] = RandomVelocity();
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pData->m_position[i] = coords[i - a_unStartID];
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pData->m_lifetime[i] = m_fLifetime;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pData->m_color[i] = m_color;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    std::size_t wIndex = a_pPool->TakeLightIndex();
    if (wIndex <= light::module::MAX_LIGHTS) {
      a_pPool->pData->m_lightIndex[i] = wIndex;
      light::Light& wLight = light::module::GetLightRef(
        a_pPool->pData->m_lightIndex[i]);
      wLight.position = glm::vec4(m_spawnLocation, 0.0);
      wLight.color = LIGHT_COLOR;
      wLight.intensity = 0.1f;
      wLight.radius = 2.0f;
    }
  }
}

glm::f32vec3 WaveEmitter::RandomVelocity() const {
  return m_spawnVelocity + glm::f32vec3(
   0.0f,
   0.0f,
   RandomFloat(-m_fVelocityRandomization, m_fVelocityRandomization)
  );
}
} /* namespace wave_project */
} /* namespace particle */
} /* namespace gem */
