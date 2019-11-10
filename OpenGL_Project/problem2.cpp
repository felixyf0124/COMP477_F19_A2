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

#include "common.hpp"

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

		glm::mat<2, 2, GLfloat> jacobianInverse;
		if (glm::determinant(JJT) != 0)
			jacobianInverse = glm::inverse(JJT);
		else
			jacobianInverse = glm::mat<2, 2, GLfloat>(0); // If matrix has no determinant, our arm is fully stretched, so safe to assume no angles?

		jacobianPseudoinverse = JJT * jacobianInverse;
	}

	glm::vec2 rotatePoint(glm::vec2 point, float angle)
	{
		float xPos = point.x * cos(angle) - point.y * sin(angle);
		float yPos = point.y * cos(angle) - point.x * sin(angle);
		return glm::vec2(xPos, yPos);
	}

	void incrementAngles(float deltaX, float deltaY)
	{
		calculateJacobian();
		calculateJacobInverse();

		glm::vec2 deltas = glm::vec2(deltaX, deltaY);
		glm::vec3 angles = jacobianPseudoinverse * deltas;

		// Add up angles:
		alpha += angles.x;
		beta += angles.y;
		omega += angles.z;

		// Get new joint positions from the change:
		glm::vec2 offset1 = baseLoc + glm::vec2(A, 0);
		joint1Pos = rotatePoint(offset1, alpha);

		glm::vec2 offset2 = joint1Pos + glm::vec2(B, 0);
		joint2Pos = rotatePoint(offset2, alpha + beta);

		glm::vec2 offset3 = joint2Pos + glm::vec2(C, 0);
		joint3Pos = rotatePoint(offset3, alpha + beta + omega);
	}

	// Function to ensure all the points are actually drawn at the correct distance:
	void clampPointDistances()
	{
		// Get the unit vectors for the three joints:
		glm::vec2 joint1UnitVec = (joint1Pos - baseLoc) / sqrt(pow(joint1Pos.x - baseLoc.x, 2) + pow(joint1Pos.y - baseLoc.y, 2));
		glm::vec2 joint2UnitVec = (joint2Pos - joint1Pos) / sqrt(pow(joint2Pos.x - joint1Pos.x, 2) + pow(joint2Pos.y - joint1Pos.y, 2));
		glm::vec2 joint3UnitVec = (joint3Pos - joint2Pos) / sqrt(pow(joint3Pos.x - joint2Pos.x, 2) + pow(joint3Pos.y - joint2Pos.y, 2));

		// Get the new point positions:
		joint1Pos = baseLoc + joint1UnitVec * A;
		joint2Pos = joint1Pos + joint2UnitVec * B;
		joint3Pos = joint2Pos + joint3UnitVec * C;
	}
};

void problem2()
{
	window = new OGLWindow(window_w, window_h, APP_TITLE);
	window->initialize();

	camera_position = glm::vec3(0.0f, 0.0, 20.0f);
	camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);


	//create shaders
	vector<OGLShader> shaderList = createShaders();
	

	model = glm::mat4(1.0f);
	projection = glm::perspective(zoom, (GLfloat)window->getBufferWidth() / window->getBufferHeight(), 0.1f, 100.0f);

	// Set drawing for larger points and lines:
	glPointSize(10.0f);
	glLineWidth(3.0f);

	// Set up the LinkedStructure:
	LinkedStructure arm;
	arm.A = 2;
	arm.B = 2;
	arm.C = 2;
	arm.alpha = 0.1f;
	arm.beta = 0.1f;
	arm.omega = 0.1f;
	arm.baseLoc = glm::vec2(0.0f, 0.0f);

	arm.incrementAngles(0, 0); // Doesn't really increment right now... only sets up rotations
	// arm.incrementAngles(-1, -1);
	arm.clampPointDistances();


	// Check if all the joints are still the correct distance:
	float Adist = (arm.joint1Pos - arm.baseLoc).length();
	float Bdist = (arm.joint2Pos - arm.joint1Pos).length();
	float Cdist = (arm.joint3Pos - arm.joint2Pos).length();

	cout << arm.baseLoc.x << ", " << arm.baseLoc.y << endl;
	cout << arm.joint1Pos.x << ", " << arm.joint1Pos.y << endl;
	cout << arm.joint2Pos.x << ", " << arm.joint2Pos.y << endl;
	cout << arm.joint3Pos.x << ", " << arm.joint3Pos.y << endl;

	cout << arm.A << " = " << Adist << endl;
	cout << arm.B << " = " << Bdist << endl;
	cout << arm.C << " = " << Cdist << endl;

	OGLMesh* pointMesh = new OGLMesh();
	// Main loop
	while (!window->getShouldClose())
	{
		// Get & handle user input events
		glfwPollEvents();

		// Clear window 
		glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shaderList[0].useShader();

		//render part

		projection_loc = shaderList[0].getProjectionLoc();
		model_loc = shaderList[0].getModelLoc();
		view_loc = shaderList[0].getViewLoc();
		color_loc = shaderList[0].getColorLoc();


		view = glm::lookAt(camera_position, camera_target, camera_up);

		color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform4fv(color_loc, 1, glm::value_ptr(color));

		// Draw the points:
		GLfloat points[] = {
			arm.baseLoc.x, arm.baseLoc.y, 0,
			arm.joint1Pos.x, arm.joint1Pos.y, 0,
			arm.joint2Pos.x, arm.joint2Pos.y, 0,
			arm.joint3Pos.x, arm.joint3Pos.y, 0,
		};

		pointMesh = new OGLMesh();
		pointMesh->createMesh(points, 0, 12, 0);

		pointMesh->drawPoints();
		pointMesh->drawLines();
		delete pointMesh;

		glUseProgram(0);

		window->swapBuffers();

	}

	glfwTerminate();

	return;
}
