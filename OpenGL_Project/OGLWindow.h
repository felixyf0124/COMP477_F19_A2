#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

class OGLWindow
{
public:
	OGLWindow();
	OGLWindow(int window_width, int window_height, string window_title);

	int initialize();

	bool getShouldClose();
	GLFWwindow* getWindow();
	int getWindowWidth();
	int getWindowHeight();
	int getBufferWidth();
	int getBufferHeight();

	void resize();
	void swapBuffers();


	~OGLWindow();
private:
	GLFWwindow* window;
	int window_width, window_height;
	int buffer_width, buffer_height;
	string window_title;

	// static void keyCallback(GLFWwindow* window, int key, int code, int action, int mode);
};

