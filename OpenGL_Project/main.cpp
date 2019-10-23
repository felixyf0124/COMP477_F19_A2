
#include <iostream>
#include <string>

#include"GL/glew.h"
#include "GLFW/glfw3.h"

using namespace std;

const char* APP_TITLE = "OpenGL";
const int window_w = 800;
const int window_h = 800;

int main()
{
	if (!glfwInit())
	{
		cerr << "GLFW initialization failed" << endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	GLFWwindow* window = glfwCreateWindow(window_w, window_h, APP_TITLE, NULL, NULL);

	if (window == NULL) 
	{
		cerr << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	// Get buffer size info
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(window); // Initialize GLEW

	// Allow modern extension features
	glewExperimental = true; // Needed in core profile
	
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Get & handle user input events
		glfwPollEvents();

		// Clear window 
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}


