#include "SkeletonJoint.h"

SkeletonJoint::SkeletonJoint(int id = -1)
{
	this->setId(id);
	this->parent = nullptr;
	this->children = new vector<SkeletonJoint*>();
	this->position = glm::vec3(0);
	this->offsetPosition = glm::vec3(0);
}

void SkeletonJoint::setId(int id)
{
	this->id = id;
}

void SkeletonJoint::setParent(SkeletonJoint* const parent)
{
	this->parent = parent;
}

/*
	this will also auto set the child's parent 
	if it is not linked with the current joint
*/
void SkeletonJoint::addChild(SkeletonJoint* const child)
{
	this->children->push_back(child);
	if (child->getParent() != this)
	{
		child->setParent(this);
	}
}

int SkeletonJoint::getId()
{
	return this->id;
}

SkeletonJoint* const SkeletonJoint::getParent()
{
	return this->parent;
}

/**
get child by id, different from index
*/
SkeletonJoint* const SkeletonJoint::getChildAtId(int id)
{
	for (GLuint i = 0; i < this->children->size(); ++i)
	{
		if (id == this->children->at(i)->getId())
		{
			return this->children->at(i);
		}
	}
	return nullptr;
}

vector<SkeletonJoint*>* const SkeletonJoint::getChildren()
{
	return this->children;
}

/**
	get a specific offspring by id under a skeleton tree
*/
SkeletonJoint* const SkeletonJoint::getOffspringById(int id)
{
	SkeletonJoint* _offspring = nullptr;
	_offspring = this->getChildAtId(id);
	if (_offspring == nullptr)
	{
		for (unsigned int i = 0; i < this->children->size(); ++i)
		{
			_offspring = this->children->at(i)->getOffspringById(id);
			if (_offspring != nullptr)
			{
				return _offspring;
			}
		}
	}
	return _offspring;
}
