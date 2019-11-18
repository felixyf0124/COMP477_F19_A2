
#include <iostream>
#include <string>
#include <vector>

#include"GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/mat4x4.hpp"
#include "NiTE\NiTE.h"
#include "NiTE\NiteCAPI.h"

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

//Prob 4 
nite::UserTracker userTracker;
nite::Status niteRc;
nite::UserTrackerFrameRef userTrackerFrame;


void niteInit() {
	niteRc = nite::NiTE::initialize();
	niteRc = userTracker.create();
}

void createShaders() {

	OGLShader* shader = new OGLShader();
	shader->createFromFileSrc(vertex_shader_path, fragment_shader_path);
	shaderList.push_back(*shader);
}


int main()
{
	window = new OGLWindow(window_w, window_h, APP_TITLE);
	window->initialize();

	camera_position = glm::vec3(5.0f, 5.0f, 1.0f);
	camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

	//glfwSetKeyCallback(window->getWindow(), keyCallback);


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
	

	//create shaders
	createShaders();


	model = glm::mat4(1.0f);
	projection = glm::perspective(zoom, (GLfloat)window->getBufferWidth() / window->getBufferHeight(), 0.1f, 100.0f);

	niteInit();

	// Main loop
	while (!window->getShouldClose())
	{


		// Get & handle user input events
		glfwPollEvents();



		// Clear window 
		glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		niteRc = userTracker.readFrame(&userTrackerFrame);

		const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();

		for (int i = 0; i < users.getSize(); ++i)
		{
			const nite::UserData& user = users[i];
			userTracker.startSkeletonTracking(user.getId());

			if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
			{
				const nite::SkeletonJoint& head =
					user.getSkeleton().getJoint(nite::JOINT_HEAD);
				if (head.getPositionConfidence() > 0.5)
				{
					printf("%d. (%5.2f, %5.2f, %5.2f)\n", user.getId(),
						head.getPosition().x,
						head.getPosition().y,
						head.getPosition().z
						);
				}
			}
		}



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

		meshList[0]->drawMesh();

		glUseProgram(0);

		window->swapBuffers();

	}

	glfwTerminate();

	return 0;
}


