#include "OGLWindow.h"

OGLWindow::OGLWindow():window_width(800), window_height(800), window_title("OpenGL")
{
}

OGLWindow::OGLWindow(int window_width, int window_height, string window_title)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window_title = window_title;

}

int OGLWindow::initialize()
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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);//resizeable

	window = glfwCreateWindow(window_width, window_height, window_title.c_str(), nullptr, nullptr);

	if (window == nullptr)
	{
		cerr << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}


	// Get buffer size info
	glfwGetFramebufferSize(window, &buffer_width, &buffer_height);

	// Set context for GLEW to use
	glfwMakeContextCurrent(window); // Initialize GLEW

	// Allow modern extension features
	glewExperimental = GL_TRUE; // Needed in core profile

	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glEnable(GL_DEPTH_TEST);
	// Setup viewport size
	glViewport(0, 0, buffer_width, buffer_height);

}

bool OGLWindow::getShouldClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* OGLWindow::getWindow()
{
	return window;
}

int OGLWindow::getWindowWidth()
{
	return window_width;
}

int OGLWindow::getWindowHeight()
{
	return window_height;
}

int OGLWindow::getBufferWidth()
{
	return buffer_width;
}

int OGLWindow::getBufferHeight()
{
	return buffer_height;
}

void OGLWindow::resize()
{

	glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
	glViewport(0, 0, buffer_width, buffer_height);
}

void OGLWindow::swapBuffers()
{
	glfwSwapBuffers(window);
}

OGLWindow::~OGLWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

//void OGLWindow::keyCallback(GLFWwindow* window, int key, int code, int action, int mode)
//{
//	
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(window, GL_TRUE);
//	}
//
//}
