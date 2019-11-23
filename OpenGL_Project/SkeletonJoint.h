#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include "Constants.h"
#include <ctime>

using namespace std;

class SkeletonJoint
{
public:
	SkeletonJoint(int id = -1);

	void setId(int id);
	void setPosition(glm::vec3 position);
	void setOffsetPosition(glm::vec3 offsetPosition);

	void setParent(SkeletonJoint* const parent);
	void setQuatLocal(glm::quat quatLocal);
	void addChild(SkeletonJoint* const child);

	int getId();
	glm::vec3 getPosition();
	glm::quat getQuatLocal();

	SkeletonJoint* const getParent();
	SkeletonJoint* const getChildAtId(int id);

	vector<SkeletonJoint*>* const getChildren();

	SkeletonJoint* const convert16To18Joints();

	SkeletonJoint* const getOffspringById(int id);

	vector<glm::vec3>* getAllJoints();

	vector<GLuint>* getSkeletonIndices();

	SkeletonJoint* const getRootJoint();

	void cookQuaternion();
	glm::quat cookQuaternion(glm::vec3 A, glm::vec3 B, glm::vec3 C);
	void cookAllQuaternion();

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

