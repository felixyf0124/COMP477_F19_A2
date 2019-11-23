#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

using namespace std;

class OGLShader
{
public:
	OGLShader();

	void createFromStrSrc(const char* vertex_code, const char* fragment_code);
	void createFromFileSrc(const char* vertex_loc, const char* fragment_loc);
	
	std::string readFile(const char* file_loc);

	void useShader();
	void clearShader();

	GLuint getModelLoc();
	GLuint getViewLoc();
	GLuint getProjectionLoc();
	GLuint getColorLoc();

	~OGLShader();

private:
	GLuint ID, projection_loc, model_loc, view_loc, color_loc;

	void compile(const char* vertex_code, const char* fragment_code);
	void addShader(GLuint program, const char* shader_code, GLenum shader_type);
	
	//
};

