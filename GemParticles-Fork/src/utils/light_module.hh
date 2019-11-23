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
#ifndef LIGHT_MODULE_HH
#define LIGHT_MODULE_HH

#include "glm/glm.hpp"

namespace gem { namespace particle {
namespace light {
// Requires padding at the end in order to be used
// in a SSBO. Also according to this post https://stackoverflow.com/questions/38172696/should-i-ever-use-a-vec3-inside-of-a-uniform-buffer-or-shader-storage-buffer-o
// vec3 should never be used in SSBO. (Maybe this is due to std140?)
// Maybe I could also use some padding at the end of each vec3 to emulate the gpu side
struct Light {
  glm::vec4  position;
  glm::vec4  color;
  float      intensity;
  float      attenuation;
  float      radius;
  float      padding;
};
struct SpotLight {
  glm::f32vec3  position;
  glm::f32vec3  color;
  float         intensity;
  float         attenuation;
  float         distance;
  float         coneAngle;
  glm::f32vec3  direction;
};
struct Material {
  glm::f32vec3  ambientFactor;
  glm::f32vec3  diffuseFactor;
  glm::f32vec3  specularFactor;
  float         shininessFactor;
};
namespace module {
/* CONSTANT THAT DEFINES THE SYSTEM LIGHT MAXIMAL CAPACITY */
constexpr std::size_t MAX_LIGHTS = 1000;

/* Setup functions */
void Init();
void Terminate();

/* These methods must be used in order for the module to work*/
// Resize should be used at the beginning of the application, 
// equivalent to std::vector::resize
void Resize(std::size_t LightCount, const Light& defaultLight = Light());
// FlushDataToGPU should be used at the end of each iteration
void FlushDataToGPU();
// SetDirty is called when one don't want to call FlushDataToGPU right away, 
// but wants to eventually send new data to GPU
void SetDirty();

/* Alters the lights count. The AddLight functions return the ID of the lights added. */
std::size_t AddLight(const Light &light);
std::size_t AddLight(Light&& light);

/* Getters */
std::size_t GetLightsCount();
Light  GetLight(std::size_t lightID);
Light& GetLightRef(std::size_t lightID);
Light* GetLightPointer(std::size_t lightID);

/* By using the light ID, one can update the corresponding light in 
the module if he or she used GetLight instead of the reference getters. */
void UpdateLight(std::size_t lightID, const Light &light);
void UpdateLight(std::size_t lightID, Light &&light);
} /* namespace module */
} /* namespace light */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: LIGHT_MODULE_HH */

