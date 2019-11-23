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
#include "emitters/random_fountain_emitter.hh"

#include <cstdlib>
#include <algorithm>

namespace gem {
namespace particle {
namespace {
const float TWO_PI = 6.28319f;
float RandomFloat(float a_fMin, float a_fMax) {
  return a_fMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (a_fMax - a_fMin)));
}

//TODO: If this gonna be reused, change the following so that
// it can be paramterized, random should be seeded, see if it can be
// optimized/changed for something cleaner/more C++11
const glm::f32vec3 RandomCircularVelocity() {
  float theta = RandomFloat(0.0f, TWO_PI);
  float dist  = RandomFloat(0.0f, 1.0f);
  return { cos(theta), 1.0f, sin(theta) };
}
}

RandomFountainEmitter::RandomFountainEmitter(const glm::f32vec3& a_spawnLocation,
  float a_fLifetime, double a_dEmissionRate, float velocity, const glm::u8vec4& a_initialColor)
  : Emitter(a_spawnLocation, { 0.0f, 0.0f, 0.0f }, a_fLifetime, a_dEmissionRate),
    m_initialColor(a_initialColor), m_velocity(velocity) { }

void RandomFountainEmitter::Init(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_velocity[i] = m_velocity * RandomCircularVelocity();
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_position[i] = m_spawnLocation;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_lifetime[i] = m_fLifetime;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_color[i] = m_initialColor;
  }
}
} /* namespace particle */
} /* namespace gem */
