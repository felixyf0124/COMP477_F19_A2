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
#ifndef PROXIMITY_COLOR_UPDATER
#define PROXIMITY_COLOR_UPDATER

#include "dynamics/dynamic.hh"
#include "core/particle_pool_core.hh"

namespace gem { namespace particle {
namespace attractor_project {
class ProximityColorUpdater : public Dynamic<CoreParticles> {
public:
  explicit ProximityColorUpdater(
    const glm::f32vec3 &a_fvPositionOfInterest,
    const glm::vec4 &a_fvHotColor,
    const glm::vec4 &a_fvColdColor,
    float a_fMaxDistance = 100.0f);
  ~ProximityColorUpdater();

  void SetPOI(const glm::f32vec3& a_vPOI) { m_fvPositionOfInterest = a_vPOI; }

  const glm::vec4& GetColdColor() const { return *m_vColdColor; }
  const glm::vec4& GetHotColor() const { return *m_vHotColor; }

  void UpdateColorGradient();
  void Update(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) override;
private:
	glm::f32vec3	            m_fvPositionOfInterest;
  float                     m_fMaxDistance;
  std::vector<glm::vec4>    m_vColorGradient;
  glm::vec4              *m_vColdColor, *m_vHotColor;

}; /* class ParticleAttractor*/
} /* namespace attractor_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PROXIMITY_COLOR_UPDATER */
