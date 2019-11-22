
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

int phase = 0;
glm::vec3 animate_points(glm::vec3 cubePoint, glm::vec3 spherePoint, float currentPhaseTimeSpan, float timePerPhase)
{
	// Get the fraction of the current line traversed:
	float timeFraction = fmod(currentPhaseTimeSpan / timePerPhase,1);
	int phase = currentPhaseTimeSpan / timePerPhase;

	if (phase == 0) // Cube to sphere:
	{
		cout << timeFraction << endl;
		// Get location at given fraction of the way between current point and next point:
		float x_diff = (spherePoint.x - cubePoint.x) * timeFraction;
		float y_diff = (spherePoint.y - cubePoint.y) * timeFraction;
		float z_diff = (spherePoint.z - cubePoint.z) * timeFraction;

		// Set position to new point:
		glm::vec3 newPos = cubePoint + glm::vec3(x_diff, y_diff, z_diff);
		return newPos;
	}
	else if (phase == 1) // Stay on sphere:
	{
		cout << 1 << endl;
		return spherePoint;
	}
	else if (phase == 2) // Sphere to cube
	{
		cout << 1-timeFraction << endl;
		// Get location at given fraction of the way between current point and next point:
		float x_diff = (spherePoint.x - cubePoint.x) * (1-timeFraction);
		float y_diff = (spherePoint.y - cubePoint.y) * (1-timeFraction);
		float z_diff = (spherePoint.z - cubePoint.z) * (1-timeFraction);
		cout << x_diff << ", " << y_diff << ", " << z_diff << endl;

		// Set position to new point:
		glm::vec3 newPos = cubePoint + glm::vec3(x_diff, y_diff, z_diff);
		return newPos;
	}
	else if (phase == 3)
	{
		cout << 0 << endl;
		return cubePoint;
	}
}

static void keyCallback(GLFWwindow* window, int key, int code, int action, int mode)
{
	// Camera Movement:
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		camera_position.x -= 1.0f;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		camera_position.x += 1.0f;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		camera_position.z -= 1.0f;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		camera_position.z += 1.0f;
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		camera_position.y += 1.0f;
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		camera_position.y -= 1.0f;
	}
}

void problem1()
{
	window = new OGLWindow(window_w, window_h, APP_TITLE);
	window->initialize();

	camera_position = glm::vec3(5.0f, 5.0, 5.0f);
	camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

	glfwSetKeyCallback(window->getWindow(), keyCallback);

	// Q1 values:
	float pointlist[] =
	{
		// Control Points:
		-1.00, -1.00, 1.00, // 0
		-1.00, -1.00, -1.00, // 1
		1.00, -1.00, -1.00, // 2
		1.00, -1.00, 1.00, // 3
		-1.00, 1.00, 1.00, // 4
		1.00, 1.00, 1.00, // 5
		1.00, 1.00, -1.00, // 6
		-1.00, 1.00, -1.00, // 7

		0.00, -1.00, 1.00, // 8
		0.00, -1.00, -1.00, // 9
		0.00, 1.00, 1.00, // 10
		0.00, 1.00, -1.00, // 11

		-1.00, 0.00, 1.00, // 12
		-1.00, 0.00, -1.00, // 13
		1.00, 0.00, -1.00, // 14
		1.00, 0.00, 1.00, // 15

		-1.00, -1.00, 0.00, // 16
		1.00, -1.00, 0.00, // 17
		-1.00, 1.00, 0.00, // 18
		1.00, 1.00, 0.00, // 19

		0.00, 0.00, 1.00, // 20
		0.00, 0.00, -1.00, // 21
		0.00, 1.00, 0.00, // 22
		0.00, -1.00, 0.00, // 23
		1.00, 0.00, 0.00, // 24
		-1.00, 0.00, 0.00, // 25
	};
	float sphereRadius = 2.0f;
	vector<float> centerOfCube = { 0.0f, 0.0f, 0.0f };

	GLuint sample_indices[] = {
		0, 16, 25,
		0, 12, 25,
		0, 16, 23,
		0, 8, 23,
		0, 12, 20,
		0, 8, 20,

		1, 16, 25,
		1, 13, 25,
		1, 16, 23,
		1, 9, 23,
		1, 13, 21,
		1, 9, 21,

		2, 17, 24,
		2, 14, 24,
		2, 17, 23,
		2, 9, 23,
		2, 14, 21,
		2, 9, 21,

		3, 17, 24,
		3, 15, 24,
		3, 17, 23,
		3, 8, 23,
		3, 15, 20,
		3, 8, 20,

		4, 18, 25,
		4, 12, 25,
		4, 18, 22,
		4, 10, 22,
		4, 12, 20,
		4, 10, 20,

		5, 19, 24,
		5, 15, 24,
		5, 19, 22,
		5, 10, 22,
		5, 15, 20,
		5, 10, 20,

		6, 19, 24,
		6, 14, 24,
		6, 19, 22,
		6, 11, 22,
		6, 14, 21,
		6, 11, 21,

		7, 18, 25,
		7, 13, 25,
		7, 18, 22,
		7, 11, 22,
		7, 13, 21,
		7, 11, 21,
	};
	GLuint vertics_size = sizeof(pointlist) / sizeof(*pointlist);
	GLuint indices_size = sizeof(sample_indices) / sizeof(*sample_indices);


	//create shaders
	vector<OGLShader> shaderList = createShaders();


	model = glm::mat4(1.0f);
	projection = glm::perspective(zoom, (GLfloat)window->getBufferWidth() / window->getBufferHeight(), 0.1f, 100.0f);

	// Calculating the destination points for each point:
	vector<glm::vec3> destPoints;

	// For each point in the pointsList, calculate the unit vector pointing from the cube center towards that point.
	// Then, obtain the new position of the point on the sphere:
	for (int i = 0; i < (sizeof(pointlist) / sizeof(GLfloat)) / 3; ++i) // from 0 to # of points - 1
	{
		glm::vec3 point = { pointlist[i * 3], pointlist[i * 3 + 1], pointlist[i * 3 + 2] };
		// Getting vector from cube center to point:
		glm::vec3 pointVec = glm::vec3(point[0] - centerOfCube[0], point[1] - centerOfCube[2], point[2] - centerOfCube[2]);
		// Get the distance between the point and cube center:
		GLfloat distance = sqrt(pow((pointVec.x), 2) + pow((pointVec.y), 2) + pow((pointVec.z), 2));
		// Get the unit vector:
		glm::vec3 unitVector = pointVec / distance;
		// Get the new point position:
		glm::vec3 newPoint = unitVector * sphereRadius;
		destPoints.push_back(newPoint);
	}

	// Set drawing for thicker lines:
	glPointSize(10.0f);
	// Time per phase:
	float phaseLength = 2.0f;

	// Initialize time variables:
	float startTime = glfwGetTime();
	float currentTime;
	float lastTime = glfwGetTime();
	float deltaTime = 0;
	float timeElapsed;

	float timePerPhase = 1.0f;
	// Generate OGLMesh:
	OGLMesh* cube = new OGLMesh();

	// Main loop
	while (!window->getShouldClose())
	{
		// Get the time step of the animation:
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		timeElapsed = currentTime - startTime;

		// For each point, determine the portion of the distance at that point in the animation:
		// Ideally, the animation will shift from cube to sphere for X seconds, pause for X seconds, then shift from sphere to cube again.
		float currentPoints[27 * 3];

		for (int i = 0; i < (sizeof(pointlist) / sizeof(GLfloat)) / 3; ++i) // from 0 to # of points - 1
		{
			glm::vec3 cubePoint = glm::vec3(pointlist[i * 3], pointlist[i * 3 + 1], pointlist[i * 3 + 2]);
			glm::vec3 spherePoint = glm::vec3(destPoints[i].x, destPoints[i].y, destPoints[i].z);
			glm::vec3 interpolationPoint = animate_points(cubePoint, spherePoint, fmod(timeElapsed, timePerPhase * 4), timePerPhase);

			currentPoints[i * 3] = interpolationPoint.x;
			currentPoints[i * 3 + 1] = interpolationPoint.y;
			currentPoints[i * 3 + 2] = interpolationPoint.z;
		}
		// Update cube mesh:
		
		cube = new OGLMesh();
		cube->createMesh(currentPoints, sample_indices, vertics_size, indices_size);

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

		// Draw the mesh:

		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform4fv(color_loc, 1, glm::value_ptr(color));

		cube->drawMesh();

		// Draw the points:

		color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform4fv(color_loc, 1, glm::value_ptr(color));

		cube->drawPoints();
		
		delete cube;
		glUseProgram(0);

		window->swapBuffers();

	}

	glfwTerminate();

	return;
}


