#pragma once
#ifndef WAVE_SCENE_HH
#define WAVE_SCENE_HH

#include "utils/light_module.hh"

namespace gem { namespace particle {
namespace wave_project {
namespace scene {
void Init(bool a_isDebug = false);
void Terminate();

void UpdateMaterial(const light::Material& a_material);
light::Material GetMaterial();
bool IsDebug();
void SetDebugOption(bool a_isDebug);
void Render();
} /* namespace scene */
} /* namespace wave_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard:WAVE_SCENE_HH */

