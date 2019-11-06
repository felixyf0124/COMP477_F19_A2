
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

const char* APP_TITLE = "COMP477 Fall 2019 Assignment 2";
const int window_w = 800;
const int window_h = 800;

OGLWindow* window = nullptr;
vector<OGLShader> shaderList;
vector<OGLMesh*> meshList;


static const char* vertex_shader_path = "vertex.shader";
static const char* fragment_shader_path = "fragment.shader";

static GLuint projection_loc = 0, model_loc = 0, view_loc, color_loc = 0;

const glm::mat4 model_default = glm::mat4(1.0f);
glm::mat4 model, view, projection;
GLfloat zoom = 45;

static glm::vec3 camera_position;// = glm::vec3(0, camHeight, 0);
static glm::vec3 camera_target;// = glm::vec3(0, camHeight, -5);
static glm::vec3 camera_up = glm::vec3(0, 1, 0);
static glm::vec3 camera_direction = glm::normalize(camera_position - camera_target);

glm::vec4 color = glm::vec4();

void createShaders() {

	OGLShader* shader = new OGLShader();
	shader->createFromFileSrc(vertex_shader_path, fragment_shader_path);
	shaderList.push_back(*shader);
}

int phase = 0;
glm::vec3 animate_points(glm::vec3 cubePoint, glm::vec3 spherePoint, float currentPhaseTimeSpan, float timePerPhase)
{
	// Get the fraction of the current line traversed:
	float timeFraction = currentPhaseTimeSpan / timePerPhase;
	if (timeFraction >= 1)
	{
		phase++;
		phase %= 4;
	}

	if (phase == 0) // Cube to sphere:
	{
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
		return spherePoint;
	}
	else if (phase == 2) // Sphere to cube
	{
		// Get location at given fraction of the way between current point and next point:
		float x_diff = (spherePoint.x - cubePoint.x) * timeFraction;
		float y_diff = (spherePoint.y - cubePoint.y) * timeFraction;
		float z_diff = (spherePoint.z - cubePoint.z) * timeFraction;

		// Set position to new point:
		glm::vec3 newPos = spherePoint - glm::vec3(x_diff, y_diff, z_diff);
		return newPos;
	}
	else if (phase == 3)
	{
		return cubePoint;
	}
}

void problem1()
{
	window = new OGLWindow(window_w, window_h, APP_TITLE);
	window->initialize();

	camera_position = glm::vec3(5.0f, 0.0, 0.0f);
	camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

	//glfwSetKeyCallback(window->getWindow(), keyCallback);

	// Q1 values:
	float pointlist[] =
	{
		-1.00, -1.00, 1.00,
		-1.00, -1.00, -1.00,
		1.00, -1.00, -1.00,
		1.00, -1.00, 1.00,
		-1.00, 1.00, 1.00,
		1.00, 1.00, 1.00,
		1.00, 1.00, -1.00,
		-1.00, 1.00, -1.00,

		0.00, -1.00, 1.00,
		0.00, -1.00, -1.00,
		0.00, 1.00, 1.00,
		0.00, 1.00, -1.00,

		-1.00, 0.00, 1.00,
		-1.00, 0.00, -1.00,
		1.00, 0.00, -1.00,
		1.00, 0.00, 1.00,

		-1.00, -1.00, 0.00,
		1.00, -1.00, 0.00,
		-1.00, 1.00, 0.00,
		1.00, 1.00, 0.00,

		0.00, 0.00, 1.00,
		0.00, 0.00, -1.00,
		0.00, 1.00, 0.00,
		0.00, -1.00, 0.00,
		0.00, 0.00, -1.00,
		0.00, 0.00, 1.00,
		1.00, 0.00, 0.00,
		-1.00, 0.00, 0.00,
	};
	float sphereRadius = 2.0f;
	vector<float> centerOfCube = { 0.0f, 0.0f, 0.0f };

	GLuint sample_indices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		0, 3, 5,
		5, 4, 0,
		3, 2, 6,
		6, 5, 3,
		2, 1, 7,
		7, 6, 2,
		1, 0, 4,
		4, 7, 1
	};
	GLuint vertics_size = sizeof(pointlist) / sizeof(*pointlist);
	GLuint indices_size = 36;


	//create shaders
	createShaders();


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
			glm::vec3 interpolationPoint = animate_points(cubePoint, spherePoint, fmod(timeElapsed, timePerPhase), timePerPhase);

			currentPoints[i * 3] = interpolationPoint.x;
			currentPoints[i * 3 + 1] = interpolationPoint.y;
			currentPoints[i * 3 + 2] = interpolationPoint.z;
		}
		// Update cube mesh:
		//cube->createMesh(currentPoints, sample_indices, vertics_size, indices_size);
		//meshList.push_back(cube);

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

		// meshList[0]->drawPoints();

		// Clean up meshList:
		// meshList[0]->clearMesh();
		// meshList.pop_back();

		glUseProgram(0);

		window->swapBuffers();

	}

	glfwTerminate();

	return;
}


