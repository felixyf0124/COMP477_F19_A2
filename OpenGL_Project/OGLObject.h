#pragma once

#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "OGLMesh.h"

#include "Collider.h"
#include "objloader.hpp"
using namespace std;

class Collider; // added forward declaration

class OGLObject
{
public:
	OGLObject();
	OGLObject(const vector<GLfloat>& vertices, const vector<GLuint>& indices);
	void addMesh(const GLfloat* vertices, const GLuint* indices, const GLuint vertices_size, const GLuint indices_size);
	void addMesh(const vector<GLfloat>& vertices, const vector<GLuint>& indices);
	void addMesh(const char* path);

	void addCollider(Collider* const collider);
	void setCollisionDamper(GLfloat collision_damper);
	void setTranslateVelocity(glm::vec3 translate_velocity);
	void setTranslateAcceleration(glm::vec3 acceleration);
	void setTranslateArritubes(glm::vec3 translate_velocity, glm::vec3 acceleration);

	//inital position
	void translateOffset(const glm::vec3& translate_offset);
	void rotateOffset(const GLfloat rotate_offset_angle, const glm::vec3& rotate_offset_axis);
	void rotateOffset(glm::mat4 rotate_offset);
	void scaleOffset(const glm::vec3& scale_offset);

	void translate();
	void translate(const GLfloat delta_time);

	void translateAlongLines(vector<glm::vec3>* points, GLfloat speed);

	void updateMotionVelocity(glm::vec3 translate_velocity);
	void updateTranslatePosition(glm::vec3 position);

	void resetCurrentSection();
	void resetTravelledDistance();
	
	GLfloat getTravelledDistance();
	glm::vec3 getTranslateVelocity();


	void detectCollisions();

	void render(const GLuint model_loc,
		const GLuint color_loc, const glm::vec4& color,
		const GLuint projection_loc, const glm::mat4& projection);

	Collider* getCollider();
	vector<glm::vec3>* getBoundingBox();

	// TO DO 
	// add child obj vector variable as component obj so can manipulate as a whole body

private:
	int id;
	vector<GLfloat> vertices;
	vector<GLuint> indices;
	vector<OGLMesh*> meshes;
	glm::mat4 model, model_translate, model_rotate, model_scale,
		model_translate_offset, model_rotate_offset, model_scale_offset;
	glm::vec4 color;

	glm::vec3 translate_velocity, acceleration;

	GLfloat travelled_distance;

	Collider* collider;

	int current_section;

	void updateColliderMotionVelocity();
	void updateColliderPosition();
};

