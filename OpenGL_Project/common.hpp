#pragma once

#include <iostream>
#include <string>
#include <vector>

#include"GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/mat4x4.hpp"

#include "OGLShader.h";
#include "OGLWindow.h";
#include "OGLMesh.h"

static const char* APP_TITLE = "COMP477 Fall 2019 Assignment 2";
static const int window_w = 800;
static const int window_h = 800;

static OGLWindow* window = nullptr;
static vector<OGLShader> shaderList;
static vector<OGLMesh*> meshList;

static const char* vertex_shader_path = "vertex.shader";
static const char* fragment_shader_path = "fragment.shader";

static GLuint projection_loc = 0, model_loc = 0, view_loc, color_loc = 0;

static const glm::mat4 model_default = glm::mat4(1.0f);
static glm::mat4 model, view, projection;
static GLfloat zoom = 45;

static glm::vec3 camera_position;// = glm::vec3(0, camHeight, 0);
static glm::vec3 camera_target;// = glm::vec3(0, camHeight, -5);
static glm::vec3 camera_up = glm::vec3(0, 1, 0);
static glm::vec3 camera_direction = glm::normalize(camera_position - camera_target);

static glm::vec4 color = glm::vec4();

vector<OGLShader> createShaders();