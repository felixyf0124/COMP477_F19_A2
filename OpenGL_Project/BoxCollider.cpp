#include "BoxCollider.h"

BoxCollider::BoxCollider(): collider_type(BOX), 
bounding_box( new vector<glm::vec3>()), 
bounding_box_reset(new vector<glm::vec3>()), 
rotation_offset(glm::mat4(1.0f))
{
}

BoxCollider::BoxCollider(glm::vec3* position)
{
	BoxCollider();
	setPosition(*position);
}

void BoxCollider::setBoundingBox(glm::vec3 max, glm::vec3 min)
{
	this->max = max;
	this->min = min;
	//bounding_box = NULL;
	generateBoudningBox();
}

void BoxCollider::setRotationOffset(glm::mat4 rotation_offset)
{
	this->rotation_offset = rotation_offset;
	updateBoundingBoxWithRotationOffset();
}

vector<glm::vec3>* BoxCollider::getBoundingBox()
{
	

	return bounding_box;
}

//glm::mat4 BoxCollider::getRotationOffset()
//{
//	return rotation_offset;
//}

bool BoxCollider::detectCollision(OGLObject* const object, const glm::vec3& direction)
{
	return false;
}

void BoxCollider::generateBoudningBox()
{
	
	vector<glm::vec3> _bounding_box;
	// + x
	_bounding_box.push_back(glm::vec3(max.x, max.y, max.z));
	_bounding_box.push_back(glm::vec3(max.x, min.y, max.z));
	_bounding_box.push_back(glm::vec3(max.x, min.y, min.z));
	_bounding_box.push_back(glm::vec3(max.x, max.y, min.z));
	// + y
	_bounding_box.push_back(glm::vec3(max.x, max.y, max.z));
	_bounding_box.push_back(glm::vec3(max.x, max.y, min.z));
	_bounding_box.push_back(glm::vec3(min.x, max.y, min.z));
	_bounding_box.push_back(glm::vec3(min.x, max.y, max.z));
	// + z
	_bounding_box.push_back(glm::vec3(max.x, max.y, max.z));
	_bounding_box.push_back(glm::vec3(min.x, max.y, max.z));
	_bounding_box.push_back(glm::vec3(min.x, min.y, max.z));
	_bounding_box.push_back(glm::vec3(max.x, min.y, max.z));
	// - x
	_bounding_box.push_back(glm::vec3(min.x, min.y, min.z));
	_bounding_box.push_back(glm::vec3(min.x, min.y, max.z));
	_bounding_box.push_back(glm::vec3(min.x, max.y, max.z));
	_bounding_box.push_back(glm::vec3(min.x, max.y, min.z));
	// - y
	_bounding_box.push_back(glm::vec3(min.x, min.y, min.z));
	_bounding_box.push_back(glm::vec3(max.x, min.y, min.z));
	_bounding_box.push_back(glm::vec3(max.x, min.y, max.z));
	_bounding_box.push_back(glm::vec3(min.x, min.y, max.z));
	// - z
	_bounding_box.push_back(glm::vec3(min.x, min.y, min.z));
	_bounding_box.push_back(glm::vec3(min.x, max.y, min.z));
	_bounding_box.push_back(glm::vec3(max.x, max.y, min.z));
	_bounding_box.push_back(glm::vec3(max.x, min.y, min.z));
	
	bounding_box->clear();
	bounding_box_reset->clear();
	
	for (unsigned int i = 0; i < _bounding_box.size() / 4; ++i) {
		
		glm::vec3 _point_1 = _bounding_box[i * 4];
		glm::vec3 _point_2 = _bounding_box[i * 4 + 1];
		glm::vec3 _point_3 = _bounding_box[i * 4 + 2];
		glm::vec3 _point_4 = _bounding_box[i * 4 + 3];
		
		//bounding_box->
		bounding_box_reset->push_back(_point_1);
		bounding_box_reset->push_back(_point_2);
		bounding_box_reset->push_back(_point_3);
		bounding_box_reset->push_back(_point_4);
		bounding_box->push_back(_point_1);
		bounding_box->push_back(_point_2);
		bounding_box->push_back(_point_3);
		bounding_box->push_back(_point_4);
		
		glm::vec3 _normal = getNormalOfPlane(_bounding_box[i * 4], _bounding_box[i * 4 + 1], _bounding_box[i * 4 + 2]);
		bounding_box_reset->push_back(_normal);
		bounding_box->push_back(_normal);
	}
	
}

void BoxCollider::updateBoundingBoxWithRotationOffset()
{
	for (unsigned int i = 0; i < bounding_box_reset->size(); ++i)
	{
		glm::vec3 _new = glm::vec3(rotation_offset * glm::vec4(bounding_box_reset->at(i), 1.0f));
		if (i % 4 != 3)
		{
			bounding_box->at(i) = _new;
		}
		else
		{
			bounding_box->at(i) = glm::normalize(_new);
		}
	}
}


