#pragma once
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include "Constants.h"

using namespace std;

class SkeletonJoint
{
public:
	SkeletonJoint(int id = -1);

	void setId(int id);
	void setPosition(glm::vec3 position);
	void setOffsetPosition(glm::vec3 offsetPosition);

	void setParent(SkeletonJoint* const parent);

	void addChild(SkeletonJoint* const child);

	int getId();
	glm::vec3 getPosition();
	SkeletonJoint* const getParent();
	SkeletonJoint* const getChildAtId(int id);

	vector<SkeletonJoint*>* const getChildren();

	SkeletonJoint* const getOffspringById(int id);

	vector<glm::vec3>* getAllJoints();

	vector<GLuint>* getSkeletonIndices();

	SkeletonJoint* const getRootJoint();

	void cookQuaternion();

	void printAll();

private:
	int id;
	SkeletonJoint* parent;
	vector<SkeletonJoint*>* children;
	glm::vec3 position;
	glm::vec3 offsetPosition;

	glm::quat quatLocal;
	glm::quat quatOffset;
};

