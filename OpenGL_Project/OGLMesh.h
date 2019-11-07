#pragma once

#include <iostream>
#include <GL/glew.h>
#include "Constants.h"

using namespace std;

class OGLMesh
{
public:
	OGLMesh();
	OGLMesh(const GLfloat* vertices, const GLuint* indices);

	
	void createMesh(const GLfloat* vertices, const GLuint* indices, GLuint vertex_size, GLuint index_size);
	void drawMesh();

	void clearMesh();
	~OGLMesh();
private:
	GLuint VAO, VBO, EBO;
	GLuint vertex_size, index_size;
	GLuint point_size, line_size;
	RenderMode render_mode;
};

