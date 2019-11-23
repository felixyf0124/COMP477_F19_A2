#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <vector>
#include <stdio.h>
#include <cstring>
#include <GL/glew.h>
#include <glm/glm.hpp>

bool loadOBJ(
	const char * path, 
	std::vector<GLfloat> & out_vertices,
	std::vector<GLuint> & out_indices
);

#endif