#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/mat4x4.hpp"

#include "OGLShader.h";
#include "OGLWindow.h";
#include "OGLMesh.h"
#include "common.hpp"

vector<OGLShader> createShaders() {

	OGLShader* shader = new OGLShader();
	shader->createFromFileSrc(vertex_shader_path, fragment_shader_path);
	shaderList.push_back(*shader);
	return shaderList;
}
