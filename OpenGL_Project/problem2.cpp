#include <iostream>
#include <string>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

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

float degToRad(float deg)
{
	return deg * M_PI / 180;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		cout << "Clicked at " << xPos << ", " << yPos << endl;

		// TODO: Convert to world coords
	}
}

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
	glm::mat<3, 2, GLfloat> jacobian;
	glm::mat<2, 3, GLfloat> jacobianPseudoinverse;

	// Calculates the jacobian matrix, and places the result in the LinkedStructure
	void calculateJacobian()
	{
		// Deriv x / deriv alpha:
		jacobian[0][0] = -A * sin(alpha) - B * sin(alpha + beta) - C * sin(alpha + beta + omega);
		jacobian[1][0] = -B * sin(alpha + beta) - C * sin(alpha + beta + omega);
		jacobian[2][0] = -C * sin(alpha + beta + omega);
		jacobian[0][1] = A * cos(alpha) + B * cos(alpha + beta) + C * cos(alpha + beta + omega);
		jacobian[1][1] = B * cos(alpha + beta) + C * cos(alpha + beta + omega);
		jacobian[2][1] = C * cos(alpha + beta + omega);
	}

	// Calculates the jacobian matrix inverse, and places the result in the LinkedStructure
	void calculateJacobInverse()
	{
		glm::mat<2, 3, GLfloat> jacobianTranspose = glm::transpose(jacobian);
		glm::mat<2, 2, GLfloat> JJT = jacobian * jacobianTranspose;

		glm::mat<2, 2, GLfloat> jacobianInverse;
		if (glm::determinant(JJT) != 0)
			jacobianInverse = glm::inverse(JJT);
		else
			jacobianInverse = glm::mat<2, 2, GLfloat>(0); // If matrix has no determinant, our arm is fully stretched, so safe to assume no angles?

		jacobianPseudoinverse = jacobianTranspose * jacobianInverse;
	}

	glm::vec2 rotatePoint(glm::vec2 origin, glm::vec2 point, float angle)
	{
		float xPos = point.x * cos(angle) - point.y * sin(angle);
		float yPos = point.y * cos(angle) - point.x * sin(angle);
		return glm::vec2(xPos + origin.x, yPos + origin.y);
	}

	void incrementAngles(float deltaX, float deltaY)
	{
		calculateJacobian();

		calculateJacobInverse();

		glm::vec2 deltas = glm::vec2(deltaX, -deltaY);
		glm::vec3 angles = jacobianPseudoinverse * deltas;

		// Add up angles:
		alpha += angles.x;
		beta += angles.y;
		omega += angles.z;

		// Get new joint positions from the change:
		glm::vec2 offset1 = glm::vec2(A, 0);
		joint1Pos = rotatePoint(baseLoc, offset1, alpha);

		glm::vec2 offset2 = glm::vec2(B, 0);
		joint2Pos = rotatePoint(joint1Pos, offset2, alpha + beta);

		glm::vec2 offset3 = glm::vec2(C, 0);
		joint3Pos = rotatePoint(joint2Pos, offset3, alpha + beta + omega);
	}

	glm::vec2 clampPoint(glm::vec2 point)
	{
		float structureRange = A + B + C; // radius of range circle;
		if (glm::length(point - baseLoc) > structureRange)
		{
			glm::vec2 unitVec = (point - baseLoc)/ glm::length(point - baseLoc);
			return unitVec * structureRange; // Furthest reachable point for the machine:
		}
		return point;
	}
};


glm::vec2 interpolation(glm::vec2 currentPoint, glm::vec2 destPoint, float currentPhaseTimeSpan, float timePerPhase)
{
	if (currentPhaseTimeSpan > timePerPhase)
		return destPoint;
	else
	{
		// Get the fraction of the current line traversed:
		float timeFraction = currentPhaseTimeSpan / timePerPhase;
		// Get location at given fraction of the way between current point and next point:
		float x_diff = (destPoint.x - currentPoint.x) * timeFraction;
		float y_diff = (destPoint.y - currentPoint.y) * timeFraction;

		// Set position to new point:
		glm::vec2 newPos = currentPoint + glm::vec2(x_diff, y_diff);
		return newPos;
	}
}

float startTime;

void problem2()
{
	window = new OGLWindow(window_w, window_h, APP_TITLE);
	window->initialize();

	camera_position = glm::vec3(0.0f, 0.0, 20.0f);
	camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Set mouse callback:
	glfwSetMouseButtonCallback(window->getWindow(), mouse_button_callback);
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

	// float deltaX = 0.015f;
	// float deltaY = -0.015f;

	glm::vec2 goalPoint = arm.clampPoint(glm::vec2(-8.0f, 0.0f));

	arm.incrementAngles(0, 0); // Doesn't really increment right now... only sets up rotations

	cout << "Current tip location: " << arm.joint3Pos.x << ", " << arm.joint3Pos.y << endl;
	// cout << "Goal tip location: " << arm.joint3Pos.x + deltaX << ", " << arm.joint3Pos.y + deltaY << endl;

	// arm.incrementAngles(deltaX, deltaY);


	// Check if all the joints are still the correct distance:
	float Adist = (arm.joint1Pos - arm.baseLoc).length();
	float Bdist = (arm.joint2Pos - arm.joint1Pos).length();
	float Cdist = (arm.joint3Pos - arm.joint2Pos).length();

	cout << "Length A: " << sqrt(pow(arm.joint1Pos.x - arm.baseLoc.x, 2) + pow(arm.joint1Pos.y - arm.baseLoc.y, 2)) << endl;
	cout << "Length B: " << sqrt(pow(arm.joint2Pos.x - arm.joint1Pos.x, 2) + pow(arm.joint2Pos.y - arm.joint1Pos.y, 2)) << endl;
	cout << "Length C: " << sqrt(pow(arm.joint3Pos.x - arm.joint2Pos.x, 2) + pow(arm.joint3Pos.y - arm.joint2Pos.y, 2)) << endl;

	OGLMesh* pointMesh = new OGLMesh();

	// Initialize time variables:
	startTime = glfwGetTime();
	float currentTime;
	float lastTime = glfwGetTime();
	float deltaTime = 0;
	float timeElapsed;

	float timePerPhase = 5.0f;

	glm::vec2 initialTipPos = arm.joint3Pos;

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

		// Apply the per-frame animation:
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		timeElapsed = currentTime - startTime;

		// Get the delta:
		glm::vec2 interpolatedPoint = interpolation(initialTipPos, goalPoint, timeElapsed, timePerPhase);
		float deltaX = interpolatedPoint.x - arm.joint3Pos.x;
		float deltaY = interpolatedPoint.y - arm.joint3Pos.y;
		arm.incrementAngles(deltaX, deltaY);

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
