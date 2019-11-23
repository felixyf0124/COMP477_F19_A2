
#include <iostream>
#include <string>
#include <vector>

#include"GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/mat4x4.hpp"
#include "glm\gtx\quaternion.hpp"

#include "OGLShader.h";
#include "OGLWindow.h";
#include "OGLMesh.h"
#include "OGLLine.h"

#include "RawDataLoader.h"

using namespace std;

const char* APP_TITLE = "COMP477 Fall 2019 Assignment 2";
const int window_w = 1000;
const int window_h = 1000;

OGLWindow* window = nullptr;
vector<OGLShader> shaderList;
vector<OGLMesh*> meshList;

int question_number = 4;

static const char* vertex_shader_path = "vertex.shader";
static const char* fragment_shader_path = "fragment.shader";

static GLuint projection_loc = 0, model_loc = 0, view_loc, color_loc = 0;

const glm::mat4 model_default = glm::mat4(1.0f);
glm::mat4 model, view, projection;
GLfloat zoom = 45;

vector<int>* atFrame = new vector<int>();

static glm::vec3 camera_position;// = glm::vec3(0, camHeight, 0);
static glm::vec3 camera_target;// = glm::vec3(0, camHeight, -5);
static glm::vec3 camera_up = glm::vec3(0, 1, 0);
static glm::vec3 camera_direction = glm::normalize(camera_position - camera_target);

const glm::mat4 turn_left_mat = glm::rotate(model_default, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
const glm::mat4 turn_right_mat = glm::rotate(model_default, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

bool isLeftKeyDown = false, isRightKeyDown = false;

glm::vec4 color = glm::vec4();


void keyCallback(GLFWwindow* window, int key, int code, int action, int mode);


void createShaders() {

	OGLShader* shader = new OGLShader();
	shader->createFromFileSrc(vertex_shader_path, fragment_shader_path);
	shaderList.push_back(*shader);
}


void problem4()
{
	window = new OGLWindow(window_w, window_h, APP_TITLE);
	window->initialize();

	glfwSetKeyCallback(window->getWindow(), keyCallback);

	camera_position = glm::vec3(5.0f, 1.0f, 1.0f);
	camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

	//glfwSetKeyCallback(window->getWindow(), keyCallback);

	atFrame->push_back(0);
	GLfloat sample_vertices[] = {
		-1.00, -1.00, 1.00,
		-1.00, -1.00, -1.00,
		1.00, -1.00, -1.00,
		1.00, -1.00, 1.00,
		-1.00, 1.00, 1.00,
		1.00, 1.00, 1.00,
		1.00, 1.00, -1.00,
		-1.00, 1.00, -1.00
	};

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
	GLuint vertics_size = 24;
	GLuint indices_size = 36;
	OGLMesh* cube = new OGLMesh();
	cube->createMesh(sample_vertices, sample_indices, vertics_size, indices_size);
	meshList.push_back(cube);

	//printf("here 98 main\n");
	RawDataLoader* dataLoader = new RawDataLoader();
	//vector<SkeletonJoint*>* _raw_data = dataLoader->loadRawData("raw_data_1");
	vector<SkeletonJoint*>* _raw_data = dataLoader->loadRawData("raw_data_1");
	vector<SkeletonJoint*>* _joint_18_data = new vector<SkeletonJoint*>();


	vector<OGLLine*>* ske_anima = new vector<OGLLine*>();
	_raw_data->at(0)->printAll();
	for (GLuint i = 0; i < _raw_data->size(); ++i)
	{
		//_raw_data->at(i)->printAll();
		OGLLine* _line = new OGLLine();

		_line->setPoints(_raw_data->at(i)->getAllJoints(), _raw_data->at(i)->getSkeletonIndices());
		ske_anima->push_back(_line);
		_joint_18_data->push_back(_raw_data->at(i)->convert16To18Joints());
	}
	cout << endl;
	_joint_18_data->at(0)->printAll();
	cout << endl;

	dataLoader->animaTestDumper(_joint_18_data);

	//create shaders
	createShaders();


	model = glm::mat4(1.0f);
	projection = glm::perspective(zoom, (GLfloat)window->getBufferWidth() / window->getBufferHeight(), 0.1f, 5000.0f);



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


		if (isLeftKeyDown)
		{
			camera_position = glm::vec3(turn_left_mat * glm::vec4(camera_position, 0.0f));
		}
		if (isRightKeyDown) {
			camera_position = glm::vec3(turn_right_mat * glm::vec4(camera_position, 0.0f));
		}



		view = glm::lookAt(camera_position, camera_target, camera_up);

		color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform4fv(color_loc, 1, glm::value_ptr(color));

		ske_anima->at(atFrame->at(0) / 2)->drawPoints();
		ske_anima->at(atFrame->at(0) / 2)->drawLines();
		++atFrame->at(0);
		atFrame->at(0) %= ske_anima->size() * 2;
		//meshList[0]->drawMesh();

		glUseProgram(0);

		window->swapBuffers();

	}

	glfwTerminate();

}

void keyCallback(GLFWwindow* window, int key, int code, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		isLeftKeyDown = true;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		isLeftKeyDown = false;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		isRightKeyDown = true;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		isRightKeyDown = false;
	}




}



