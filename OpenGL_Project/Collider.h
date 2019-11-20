#pragma once

#include "OGLObject.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

class OGLObject; // added forward declaration

enum ColliderType
{
	BOX, SPHERE
};

class Collider
{
public:
	Collider();
	Collider(OGLObject* const object, bool is_detector);

	void attach(OGLObject* const object);
	void setPosition(glm::vec3 position);
	void setCollisionDamper(GLfloat collision_damper);
	virtual void setRotationOffset(glm::mat4 rotation_offset) = 0;
	void setObjectList(vector<OGLObject*>* object_list);
	void setIsEnableCollisionDetection(bool is_enable);
	void setIsEnableCollisionReflection(bool is_enable);
	void setMotionVelocity(const glm::vec3& translate_velocity);
	bool getIsEnableCollisionDetection();
	bool getIsEnableCollisionReflection();

	OGLObject* getAttechedObject();
	vector<OGLObject*>* getCollisionList();
	ColliderType getColliderType();
	glm::vec3* getPosition();
	GLfloat getCollisionDamper();
	const glm::vec3& getCenter();
	glm::vec3 getMotionVelocity();
	//virtual glm::mat4 getRotationOffset() = 0;



	virtual vector<glm::vec3>* getBoundingBox() = 0;

	glm::vec3 getNormalOfPlane(const glm::vec3& point_1, const glm::vec3& point_2, const glm::vec3& point_3);

	bool isDetected(OGLObject* const target);
	bool isEnableColisionReflection();

	void updateObjectTranslateMotionVelocity(glm::vec3 translate_direction);
	void updateObjectTranslatePosition(glm::vec3 position);
	
	virtual bool detectCollision(OGLObject* const object, const glm::vec3& direction) = 0;
	void detectCollisions(const glm::vec3& direction);

private:
	OGLObject* object;
	ColliderType collider_type;
	bool is_enable_collision_detection, is_enable_collision_reflection;
	glm::vec3 position;
	glm::vec3* last_position;
	glm::vec3 center, translate_velocity;
	vector<OGLObject*> collision_list;
	vector<OGLObject*>* object_list;

	GLfloat collision_damper;
};

