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
#ifndef BASIC_STREAM_EMITTER_HH
#define BASIC_STREAM_EMITTER_HH

#include "emitters/emitter.hh"
#include "core/particle_pool_core.hh"

namespace gem {
namespace particle {
class BasicStreamEmitter : public Emitter<CoreParticles> {
public:
  BasicStreamEmitter();
  BasicStreamEmitter(
    const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime,
    double a_dEmissionRate,
    float a_dVelocityRandomization = 0.0f,
    const glm::u8vec4& a_color = DEFAULT_COLOR);
	virtual ~BasicStreamEmitter() = default;

private:
  void Init(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) override;
  
  glm::f32vec3 RandomVelocity() const;

  float       m_fVelocityRandomization;
  glm::u8vec4 m_color;
}; /* class BasicStreamEmitter*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: BASIC_EMITTER_HH */
