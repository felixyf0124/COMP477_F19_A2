#include "SkeletonJoint.h"

SkeletonJoint::SkeletonJoint(int id)
{
	this->setId(id);
	this->parent = nullptr;
	this->children = new vector<SkeletonJoint*>();
	this->position = glm::vec3(0);
	this->offsetPosition = glm::vec3(0);
	this->quatLocal = glm::quat();
	this->quatOffset = glm::quat();
}

void SkeletonJoint::setId(int id)
{
	this->id = id;
}

void SkeletonJoint::setPosition(glm::vec3 position)
{
	this->position = position;
}

void SkeletonJoint::setOffsetPosition(glm::vec3 offsetPosition)
{
	this->offsetPosition = offsetPosition;
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
	if (this != nullptr)
	{
		this->children->push_back(child);
		if (child->getParent() != this)
		{
			child->setParent(this);
		}
	}
}

int SkeletonJoint::getId()
{
	return this->id;
}

glm::vec3 SkeletonJoint::getPosition()
{
	return this->position;
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
	if (this->children->size() > 0)
	{
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
		else
		{
			return _offspring;
		}
	}
	return nullptr;
}

vector<glm::vec3>* SkeletonJoint::getAllJoints()
{
	vector<glm::vec3>* _skeleton = new vector<glm::vec3>();
	if (this->id == 0)
	{
		_skeleton->push_back(this->position);
		while(true)
		{
			SkeletonJoint* _next = this->getOffspringById(_skeleton->size());
			if (_next != nullptr)
			{
				_skeleton->push_back(_next->getPosition());
			}
			else
			{
				return _skeleton;
			}
		}
	}
	else
	{
		SkeletonJoint* _root = this->getRootJoint();
		
		return _root->getAllJoints();
	}
}

vector<GLuint>* SkeletonJoint::getSkeletonIndices()
{
	vector<glm::vec3>* _skeleton = this->getAllJoints();
	SkeletonJoint* _root = this->getRootJoint();
	vector<GLuint>* _indices = new vector<GLuint>();
	for (GLuint i = 0; i < _skeleton->size()-1; ++i)
	{
		SkeletonJoint* _joint = _root->getOffspringById(i + 1);
		if (_joint != nullptr)
		{
			_indices->push_back(_joint->getParent()->getId());
			_indices->push_back(_joint->getId());
			_joint = nullptr;
		}
		else 
		{
			_joint = nullptr;
			return _indices;
		}
	}
	return _indices;
}

SkeletonJoint* const SkeletonJoint::getRootJoint()
{
	
	if (this->getId() == 0)
	{
		return this;
	}
	else if (this->getParent()->getId() != -1)
	{
		return this->getParent()->getRootJoint();
	}
	else
	{
		return nullptr;
	}
}

void SkeletonJoint::cookQuaternion()
{
	if (this->id != 0) {
		glm::vec3 A = this->getParent()->getPosition();
		glm::vec3 B = this->getPosition();
		//glm::vec3 C = 
		//this->quatLocal = special::quatFromTwoVec(_parent,)
	}
}

/*
	this is for debugging only
*/
void SkeletonJoint::printAll()
{
	cout << this->id << " -> (";
	for (GLuint i = 0; i < this->children->size(); ++i)
	{
		if (i > 0)
		{
			cout << " | ";
		}
		this->children->at(i)->printAll();
	}
	cout << ")";
}
