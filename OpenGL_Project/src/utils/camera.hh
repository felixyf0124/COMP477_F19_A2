/*************************************************************************
 * Copyright (c) 2016 François Trudel
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
 *
*************************************************************************/
#ifndef CAMERA_HH
#define CAMERA_HH

#include <map>
#include <string>

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace camera {
void Init();
void Terminate();

const glm::vec3& GetEyePosition();
const glm::vec3& GetTargetPosition();
const glm::vec3& GetUpVector();

const glm::mat4& GetViewMatrix();
const glm::mat4& GetProjectionMatrix();

void SetEyePosition(const glm::vec3& a_vEye);
void SetTargetPosition(const glm::vec3& a_vTarget);
void SetUpVector(const glm::vec3& a_vUp);

void SetViewMatrix(const glm::mat4& a_ViewMatrix);

void LookAt(const glm::vec3& a_Eye,
  const glm::vec3& a_Target,
  const glm::vec3& a_Up);

void SetProjectionMatrix(const glm::mat4& a_ProjectionMatrix);

void SetPerspectiveProjection(float a_fFOV, float a_fWidth,
  float a_fHeight, float a_fNear, float a_fFar);

void SetOrthoProjection(float a_fLeft, float a_fRight,
  float a_fBottom, float a_fTop,
  float a_fNear, float a_fFar);

} /* namespace camera*/
#endif /* end of include guard: CAMERA_HH */
