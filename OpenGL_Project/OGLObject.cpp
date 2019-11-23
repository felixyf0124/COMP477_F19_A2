#include "OGLObject.h"

//try not include in header to avoid include loop

OGLObject::OGLObject():
	model(glm::mat4(1.0f)), 
	model_translate(glm::mat4(1.0f)),
	model_rotate(glm::mat4(1.0f)), 
	model_scale(glm::mat4(1.0f)),
	model_translate_offset(glm::mat4(1.0f)), 
	model_rotate_offset(glm::mat4(1.0f)),
	model_scale_offset(glm::mat4(1.0f)), 
	translate_velocity(glm::vec3(0.0f)), 
	acceleration(glm::vec3(0.0f)), 
	collider(nullptr), 
	current_section(0),
	travelled_distance(0.0f)
{
};

OGLObject::OGLObject(const vector<GLfloat>& vertices, const vector<GLuint> & indices):
	model(glm::mat4(1.0f)),
	model_translate(glm::mat4(1.0f)),
	model_rotate(glm::mat4(1.0f)),
	model_scale(glm::mat4(1.0f)),
	model_translate_offset(glm::mat4(1.0f)),
	model_rotate_offset(glm::mat4(1.0f)),
	model_scale_offset(glm::mat4(1.0f)),
	translate_velocity(glm::vec3(0.0f)),
	acceleration(glm::vec3(0.0f)),
	collider(nullptr),
	current_section(0),
	travelled_distance(0.0f)
{
	OGLObject();
	//current_section = 0;
	addMesh(vertices, indices);
}

void OGLObject::addMesh(const GLfloat* vertices, const GLuint* indices, const GLuint vertices_size, const GLuint indices_size)
{
	OGLMesh* _mesh = new OGLMesh();
	_mesh->createMesh(&vertices[0], &indices[0], vertices_size, indices_size);
	meshes.push_back(_mesh);
}

void OGLObject::addMesh(const vector<GLfloat>& vertices, const vector<GLuint>& indices)
{
	OGLMesh* _mesh = new OGLMesh();
	_mesh->createMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshes.push_back(_mesh);
}

void OGLObject::addMesh(const char* path)
{
	vector<GLfloat> _vertices;
	vector<GLuint> _indices;
	if (loadOBJ(path, _vertices, _indices) == false)
	{
		cerr << "object loading failed from " << path << endl;
	}
	addMesh(_vertices, _indices);
}

void OGLObject::addCollider(Collider* const collider)
{
	if (this->collider != collider)
	{
		this->collider = collider;
	}
	this->collider->attach(this);
}

void OGLObject::setCollisionDamper(GLfloat collision_damper)
{
	collider->setCollisionDamper(collision_damper);
}

void OGLObject::setTranslateVelocity(glm::vec3 translate_velocity)
{
	this->translate_velocity = translate_velocity;
}

void OGLObject::setTranslateAcceleration(glm::vec3 acceleration)
{
	this->acceleration = acceleration;
}

void OGLObject::setTranslateArritubes(glm::vec3 translate_velocity, glm::vec3 acceleration)
{
	this->translate_velocity = translate_velocity;
	this->acceleration = acceleration;
}

void OGLObject::translateOffset(const glm::vec3& translate_offset)
{
	model_translate_offset = glm::translate(glm::mat4(1.0f), translate_offset); 
}

void OGLObject::rotateOffset(const GLfloat rotate_offset_angle, const glm::vec3& rotate_offset_axis)
{
	model_rotate_offset = glm::rotate(glm::mat4(1.0f), rotate_offset_angle, rotate_offset_axis);
}

void OGLObject::rotateOffset(glm::mat4 rotate_offset)
{
	model_rotate_offset = rotate_offset;
	collider->setRotationOffset(rotate_offset);
}

void OGLObject::scaleOffset(const glm::vec3& scale_offset)
{
	model_scale_offset = glm::scale(glm::mat4(1.0f), scale_offset);
}

void OGLObject::translate()
{
	translate_velocity += acceleration;
	updateMotionVelocity(translate_velocity);
	model_translate = glm::translate(model_translate, translate_velocity);
	if (collider != nullptr)
	{
		updateColliderPosition();
	}
}

void OGLObject::translate(const GLfloat delta_time)
{
	updateMotionVelocity(translate_velocity);
	model_translate = glm::translate(model_translate, translate_velocity * delta_time);
	if (collider != nullptr)
	{
		updateColliderPosition();
	}
	
}

void OGLObject::translateAlongLines(vector<glm::vec3>* points, GLfloat speed)
{
	if (current_section < points->size()) {
		if (current_section == 0)
		{
			++current_section;
			updateTranslatePosition(points->at(current_section-1));
		}
		glm::vec3 _direction = glm::normalize(points->at(current_section) - points->at(current_section-1));
		glm::vec3 _last_translate = glm::vec3(model_translate * glm::vec4(glm::vec3(0.0f), 1.0f));
		glm::mat4  _current_translate_mat = glm::translate(model_translate, _direction * speed);
		glm::vec3 _current_translate = glm::vec3(_current_translate_mat * glm::vec4(glm::vec3(0.0f), 1.0f));
		glm::vec3 _travelled = _current_translate - _last_translate;
		glm::vec3 _passed = _current_translate - points->at(current_section-1);
		GLfloat _line_segment_length = glm::length(points->at(current_section)
			- points->at(current_section-1));
		GLfloat _exceed = glm::length(_passed)
			- _line_segment_length;

		//within the segment
		if (_exceed < 0)
		{
			travelled_distance += glm::length(_travelled);
			model_translate = _current_translate_mat;
		}// reach the end
		else if(_exceed== 0)
		{
			travelled_distance += glm::length(_travelled);

			++current_section;
			updateTranslatePosition(points->at(current_section-1));
		}
		else if (current_section < points->size() - 1)   //exceed
		{
			_travelled = points->at(current_section) - _last_translate;
			travelled_distance += glm::length(_travelled);

			++current_section;
			updateTranslatePosition(points->at(current_section-1));
			translateAlongLines(points, _exceed);
		}
		else
		{
			++current_section;
			updateTranslatePosition(points->at(current_section-1));
		}
	}
}

void OGLObject::updateMotionVelocity(glm::vec3 translate_velocity)
{
	if (collider != nullptr)
	{
		collider->setMotionVelocity(translate_velocity);
	}
	this->translate_velocity = translate_velocity;
}

void OGLObject::updateTranslatePosition(glm::vec3 position)
{
	this->model_translate = glm::translate(glm::mat4(1.0f), position);
}

void OGLObject::resetCurrentSection()
{
	current_section = 0;
}

void OGLObject::resetTravelledDistance()
{
	this->travelled_distance = 0.0f;
}

GLfloat OGLObject::getTravelledDistance()
{
	return GLfloat(travelled_distance);
}

glm::vec3 OGLObject::getTranslateVelocity()
{
	return translate_velocity;
}

void OGLObject::detectCollisions()
{
	collider->detectCollisions(translate_velocity);
}

void OGLObject::render(const GLuint model_loc, 
	const GLuint color_loc, const glm::vec4& color, 
	const GLuint projection_loc, const glm::mat4& projection)
{

	model = model_translate * model_translate_offset * 
		model_rotate * model_rotate_offset * 
		model_scale * model_scale_offset *
		glm::mat4(1.0f);
	for (unsigned int i = 0; i < meshes.size(); ++i) {
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform4fv(color_loc, 1, glm::value_ptr(color));
		meshes[i]->drawMesh();
	}
}

Collider* OGLObject::getCollider()
{
	return collider;
}

vector<glm::vec3>* OGLObject::getBoundingBox()
{
	if (collider != nullptr)
	{
		if (collider->getColliderType() == BOX)
		{
			return collider->getBoundingBox();
		}
	}
	
	return nullptr;
	
}

void OGLObject::updateColliderMotionVelocity()
{
	collider->setMotionVelocity(translate_velocity);
}

void OGLObject::updateColliderPosition()
{
	glm::vec3 _position;
	_position = glm::vec3(model_translate * glm::vec4(glm::vec3(0.0f), 1.0f));
	collider->setPosition(_position);
};




