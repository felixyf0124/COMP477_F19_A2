#include "SkeletonJoint.h"

SkeletonJoint::SkeletonJoint(int id)
{
	this->setId(id);
	this->parent = nullptr;
	this->children = new vector<SkeletonJoint*>();
	this->position = glm::vec3(0);
	this->offsetPosition = glm::vec3(0);
	this->quatLocal = glm::quat(1, 0, 0, 0);
	this->quatOffset = glm::quat(1, 0, 0, 0);
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

void SkeletonJoint::setQuatLocal(glm::quat quatLocal)
{
	this->quatLocal = quatLocal;
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

glm::quat SkeletonJoint::getQuatLocal()
{
	return this->quatLocal;
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

/*
in order to make & replay .anima, we have to convert 16 joints to the 18 joints
with the correct associated id
missing right toe at 7 with parent 6
missing left  toe at 11 with parent 10
*/
SkeletonJoint* const SkeletonJoint::convert16To18Joints()
{
	SkeletonJoint* _root_18 = new SkeletonJoint(0);
	SkeletonJoint* _root = this->getRootJoint();
	_root_18->setPosition(_root->getPosition());

	for (GLuint i = 1; i < 18; ++i)
	{
		if (i < 7)
		{
			SkeletonJoint* _joint_18 = new SkeletonJoint(i);
			SkeletonJoint* _joint_16 = _root->getOffspringById(i);
			GLuint _parent_id = _joint_16->getParent()->getId();
			if (_parent_id == 0) {
				_root_18->addChild(_joint_18);
			}
			else
			{
				_root_18->getOffspringById(_parent_id)->addChild(_joint_18);
			}
			_joint_18->setPosition(_joint_16->getPosition());
		}
		else if (i == 7)
		{
			SkeletonJoint* _right_toe = new SkeletonJoint(7);
			GLuint _parent_id = 6;
			_root_18->getOffspringById(_parent_id)->addChild(_right_toe);
		}
		else if (i < 11)
		{
			SkeletonJoint* _joint_18 = new SkeletonJoint(i);
			SkeletonJoint* _joint_16 = _root->getOffspringById(i - 1);
			GLuint _parent_id = _joint_16->getParent()->getId();
			if (_parent_id == 0) {
				_root_18->addChild(_joint_18);
			}
			else if (_parent_id < 7)
			{
				_root_18->getOffspringById(_parent_id)->addChild(_joint_18);
			}
			else if (_parent_id >= 7 && _parent_id < 11)
			{
				++_parent_id;

				_root_18->getOffspringById(_parent_id)->addChild(_joint_18);
			}
			_joint_18->setPosition(_joint_16->getPosition());
		}
		else if(i == 11)
		{
			SkeletonJoint* _left_toe = new SkeletonJoint(11);
			GLuint _parent_id = 10;
			_root_18->getOffspringById(_parent_id)->addChild(_left_toe);
		}
		else
		{
			SkeletonJoint* _joint_18 = new SkeletonJoint(i);
			SkeletonJoint* _joint_16 = _root->getOffspringById(i - 2);
			GLuint _parent_id = _joint_16->getParent()->getId();
			if (_parent_id == 0) {
				_root_18->addChild(_joint_18);
			}
			else if (_parent_id < 7)
			{
				_root_18->getOffspringById(_parent_id)->addChild(_joint_18);
			}
			else if (_parent_id >= 7 && _parent_id < 11)
			{
				++_parent_id;

				_root_18->getOffspringById(_parent_id)->addChild(_joint_18);
			}
			else if(_parent_id >= 11)
			{
				_parent_id += 2;
				_root_18->getOffspringById(_parent_id)->addChild(_joint_18);
			}
			_joint_18->setPosition(_joint_16->getPosition());
		}
	}

	return _root_18;
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
	if (this->id != 0 && this->getChildren()->size() == 1) {
		glm::vec3 A = this->getParent()->getPosition();
		glm::vec3 B = this->getPosition();
		glm::vec3 C = this->getChildren()->at(0)->getPosition();
		this->quatLocal = special::quatFromTwoVec(B - A, C - B);
	}
}

glm::quat SkeletonJoint::cookQuaternion(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	return special::quatFromTwoVec(B - A, C - B);;
}

/*
	this only functioning when this is the root joint & the joint set is coverted to joint_18
*/
void SkeletonJoint::cookAllQuaternion()
{
	if (this->id == 0 && this->getAllJoints()->size() == 18)
	{
		for (GLuint i = 1; i < this->getAllJoints()->size(); ++i)
		{
			// ignore two toe joints & center hip
			if (i != 7 && i != 11 && i != 2)
			{
				if (i == 3)
				{
					SkeletonJoint* _joint = this->getOffspringById(i);
					glm::vec3 A = this->getOffspringById(1)->getPosition();
					glm::vec3 B = this->getPosition();
					glm::vec3 C = _joint->getPosition();
					_joint->setQuatLocal(cookQuaternion(A, B, C));
				}
				else 
				{
					SkeletonJoint* _joint = this->getOffspringById(i);
					
					if (_joint->getChildren()->size() != 0)
					{
						glm::vec3 A = _joint->getParent()->getPosition();
						glm::vec3 B = _joint->getPosition();
						glm::vec3 C = _joint->getChildren()->at(0)->getPosition();
						glm::quat _quat = cookQuaternion(A, B, C);
						_joint->setQuatLocal(_quat);
					}
				}
			}
		}
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
