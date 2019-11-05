#pragma once

#include <iostream>
#include <GL/glew.h>

using namespace std;

class OGLMesh
{
public:
	OGLMesh();
	OGLMesh(const GLfloat* vertices, const GLuint* indices);

	
	void createMesh(const GLfloat* vertices, const GLuint* indices, GLuint vertex_size, GLuint index_size);
	void drawMesh();
	void drawPoints();

	void clearMesh();
	~OGLMesh();
private:
	GLuint VAO, VBO, EBO;
	GLuint vertex_size, index_size;
};

