#pragma once
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

class SkeletonJoint
{
public:
	SkeletonJoint(int id);

	void setId(int id);
	void setParent(SkeletonJoint* const parent);

	void addChild(SkeletonJoint* const child);
	int getId();
	SkeletonJoint* const getParent();
	SkeletonJoint* const getChildAtId(int id);

	vector<SkeletonJoint*>* const getChildren();

	SkeletonJoint* const getOffspringById(int id);

private:
	int id;
	SkeletonJoint* parent;
	vector<SkeletonJoint*>* children;
	glm::vec3 position;
	glm::vec3 offsetPosition;
};

