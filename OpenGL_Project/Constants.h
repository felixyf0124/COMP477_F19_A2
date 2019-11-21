#pragma once

#include <cmath>
#include <iostream>

#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"

static const double PI = 3.14159265358979323846;

static enum RenderMode
{
	POINT,LINE, FACE, POINT_AND_LINE, POINT_AND_FACE, LINE_AND_FACE, ALL
};


namespace special
{
	/*
	return quat from two vectors
	reference from
	http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
	*/
	static glm::quat quatFromTwoVec(glm::vec3 start, glm::vec3 end) {
		start = glm::normalize(start);
		end = glm::normalize(end);

		float _cos_theta = glm::dot(start, end);
		glm::vec3 _rotation_axis;

		_rotation_axis = glm::normalize(glm::cross(start, end));

		float _angle = glm::acos(_cos_theta);
		float _invs = 1 / _angle;
		float _half_sin = sin(0.5f * _angle);
		float _half_cos = cos(0.5f * _angle);

		return glm::quat(_half_cos,
			_rotation_axis.x * _half_sin,
			_rotation_axis.y * _half_sin,
			_rotation_axis.z * _half_sin);
	}
}