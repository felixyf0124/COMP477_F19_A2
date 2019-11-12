#pragma once
#include "Collider.h"

using namespace std;

class BoxCollider :
	public Collider
{
public:
	BoxCollider();
	BoxCollider(glm::vec3* position);
	void setBoundingBox(glm::vec3 max, glm::vec3 min);
	void setRotationOffset(glm::mat4 rotation_offset);
	vector<glm::vec3>* getBoundingBox();
	//glm::mat4 getRotationOffset();


	bool detectCollision(OGLObject* const object, const glm::vec3& direction);

private:
	ColliderType collider_type;
	glm::vec3 max, min;
	//vector<glm::vec3>
	vector<glm::vec3>* bounding_box, * bounding_box_reset;

	glm::mat4 rotation_offset;
	void generateBoudningBox();
	void updateBoundingBoxWithRotationOffset();
};

