#include "OGLMesh.h"

OGLMesh::OGLMesh():VAO(0), VBO(0), EBO(0)
{

}

OGLMesh::OGLMesh(const GLfloat* vertices, const GLuint* indices)
{
	//createMesh(vertices, indices);
}


void OGLMesh::createMesh(const GLfloat* vertices, const GLuint* indices, GLuint vertex_size, GLuint index_size)
{
	this->vertex_size = vertex_size;
	this->index_size = index_size;

	//cout << this->index_size << " | " << sizeof(vertices[0]) << " | " << sizeof(vertices) << endl;
	//vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vbo
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertex_size, vertices, GL_STATIC_DRAW);
	
	//ebo
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * index_size, indices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void OGLMesh::drawMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, this->index_size, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OGLMesh::drawPoints()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);

	glDrawArrays(GL_POINTS, 0, this->vertex_size / 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OGLMesh::drawLines()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);

	glDrawArrays(GL_LINE_STRIP, 0, this->vertex_size / 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OGLMesh::clearMesh()
{
	if (EBO != 0)
	{
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	index_size = 0;
}

OGLMesh::~OGLMesh()
{
	clearMesh();
}


