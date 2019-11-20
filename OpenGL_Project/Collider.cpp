#include "Collider.h"

#include<vector>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>

Collider::Collider() :
	object(nullptr), is_enable_collision_detection(false),
	position(glm::vec3(0.0f)), last_position(nullptr),
	center(glm::vec3(0.0f)), translate_velocity(glm::vec3(0.0f)), collision_damper(1.0f)
{
}

Collider::Collider(OGLObject* const object, const bool is_enable): 
	position(glm::vec3(0.0f)), last_position(nullptr),
	center(glm::vec3(0.0f)), translate_velocity(glm::vec3(0.0f))
{
	//this->type = type;
	this->object = object;
	this->is_enable_collision_detection = is_enable;
}

void Collider::attach(OGLObject* const object)
{
	if (this->object != object)
	{
		this->object = object;
	}
}

void Collider::setPosition(glm::vec3 position)
{
	//cout << "from Collider setPosition "
	//	<< "line 30 " <<
	//	endl;
	//update
	if (last_position != nullptr)
	{
		glm::vec3 _position = this->position;
		this->last_position = &_position;
		this->position = position;
	}
	else//initial
	{
		this->position = position;
		glm::vec3 _position = this->position;
		this->last_position = &_position;
	}
}

void Collider::setCollisionDamper(GLfloat collision_damper)
{
	this->collision_damper = collision_damper;
}

void Collider::setObjectList(vector<OGLObject*>* object_list)
{
	this->object_list = object_list;
}

void Collider::setIsEnableCollisionDetection(bool is_enable)
{
	this->is_enable_collision_detection = is_enable;
}

void Collider::setIsEnableCollisionReflection(bool is_enable)
{
	is_enable_collision_reflection = is_enable;
}

void Collider::setMotionVelocity(const glm::vec3& translate_velocity)
{
	//cout << "from Collider setMotionDirection "
	//	<< "line 53 " <<
	//	endl;
	this->translate_velocity = translate_velocity;
}

bool Collider::getIsEnableCollisionDetection()
{
	return is_enable_collision_detection;
}

bool Collider::getIsEnableCollisionReflection()
{
	return is_enable_collision_reflection;
}

OGLObject* Collider::getAttechedObject()
{
	return this->object;
}

vector<OGLObject*>* Collider::getCollisionList()
{
	return &collision_list;
}

ColliderType Collider::getColliderType()
{
	return collider_type;
}

glm::vec3* Collider::getPosition()
{
	return &position;
}

GLfloat Collider::getCollisionDamper()
{
	return collision_damper;
}

const glm::vec3& Collider::getCenter()
{
	return center;
}

glm::vec3 Collider::getMotionVelocity()
{
	return translate_velocity;
}

glm::vec3 Collider::getNormalOfPlane(const glm::vec3& point_1, const glm::vec3& point_2, const glm::vec3& point_3)
{
	glm::vec3 _normal = glm::normalize(glm::cross((point_1 - point_2), (point_3 - point_2)));
	return _normal;
}

bool Collider::isDetected(OGLObject* const target)
{
	for (unsigned int i = 0; i < collision_list.size(); ++i)
	{
		//cout << "from collider "
		//	<< "isDetected "
		//	<< target << " | " << collision_list[i] << " || "
		//	<< target << " | " << collision_list[i] << " || "
		//	<<endl;
		if (&target == &collision_list[i])
		{
			return true;
		}
	}
	return false;
}

bool Collider::isEnableColisionReflection()
{
	return is_enable_collision_reflection;
}

void Collider::updateObjectTranslateMotionVelocity(glm::vec3 translate_velocity)
{
	object->updateMotionVelocity(translate_velocity);
}

void Collider::updateObjectTranslatePosition(glm::vec3 position)
{
	object->updateTranslatePosition(position);
}

void Collider::detectCollisions(const glm::vec3& direction)
{
	//cout << "from Collider detectCollisions "
	//	<< "line 134 " <<
	//	endl;
	if (getIsEnableCollisionDetection())
	{
	//cout << "from collider " << "getIsEnableCollisionReflection " <<  endl;

		for (unsigned int i = 0; i < object_list->size(); ++i) {
			//cout << "from collider " 
			//	<< "isDetected " 
			//	<< object_list->at(i) << endl;

			if(!isDetected(object_list->at(i)))
			{
				if (detectCollision(object_list->at(i), direction))
				{
					collision_list.push_back(object_list->at(i));
				}
			}
		}
	}
}
