
#include <iostream>
#include <string>
#include <vector>

#include"GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/mat4x4.hpp"

#include "OGLShader.h";
#include "OGLWindow.h";
#include "OGLMesh.h"

using namespace std;

struct LinkedStructure
{
	// The location of the base:
	glm::vec2 baseLoc;

	// Angles:
	GLfloat alpha;
	GLfloat beta;
	GLfloat omega;

	// Lengths:
	GLfloat A;
	GLfloat B;
	GLfloat C;

	// Joint positions:
	glm::vec2 joint1Pos;
	glm::vec2 joint2Pos;
	glm::vec2 joint3Pos;

	// Calculated jacobian matrix:
	glm::mat<2, 3, GLfloat> jacobian;
	glm::mat<2, 3, GLfloat> jacobianPseudoinverse;

	// Calculates the jacobian matrix, and places the result in the LinkedStructure
	void calculateJacobian()
	{
		// Deriv x / deriv alpha:
		jacobian[0][0] = -A * sin(alpha) - B * sin(alpha + beta) - C * sin(alpha + beta + omega);
		jacobian[0][1] = -B * sin(alpha + beta) - C * sin(alpha + beta + omega);
		jacobian[0][2] = -C * sin(alpha + beta + omega);
		jacobian[1][0] = A * cos(alpha) + B * cos(alpha + beta) + C * cos(alpha + beta + omega);
		jacobian[1][1] = B * cos(alpha + beta) + C * cos(alpha + beta + omega);
		jacobian[1][2] = C * cos(alpha + beta + omega);
	}

	// Calculates the jacobian matrix inverse, and places the result in the LinkedStructure
	void calculateJacobInverse()
	{
		glm::mat<2, 2, GLfloat> JJT = jacobian * glm::transpose(jacobian);

		glm::mat<2, 2, GLfloat> jacobianInverse = glm::inverse(JJT);

		jacobianPseudoinverse = JJT * jacobianInverse;
	}

	void incrementAngles(float deltaX, float deltaY)
	{
		glm::vec2 deltas = glm::vec2(deltaX, deltaY);
		glm::vec3 angles = jacobianPseudoinverse * deltas;

		// Add up angles:
		alpha += angles.x;
		beta += angles.x;
		omega += angles.x;

		// Get new joint positions from the change:
		glm::vec2 offset1 = baseLoc + glm::vec2(A, 0);
		joint1Pos = glm::vec2(offset1.x * cos(alpha) - offset1.y * sin(alpha), offset1.y * cos(alpha) - offset1.x * sin(alpha));

		glm::vec2 offset2 = baseLoc + joint1Pos + glm::vec2(B, 0);
		joint2Pos = glm::vec2(offset2.x * cos(alpha + beta) - offset2.y * sin(alpha + beta), offset2.y * cos(alpha + beta) - offset2.x * sin(alpha + beta));

		glm::vec2 offset3 = baseLoc + joint1Pos + joint2Pos + glm::vec2(C, 0);
		joint2Pos = glm::vec2(offset3.x * cos(alpha + beta + omega) - offset3.y * sin(alpha + beta + omega), offset3.y * cos(alpha + beta + omega) - offset3.x * sin(alpha + beta + omega));
	}
};

void problem2()
{

}
