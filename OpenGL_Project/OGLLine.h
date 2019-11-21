#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"

using namespace std;

enum CurveMode
{
	LINEAR, INTERPOLATEION, APPROXIMATION
};

class OGLLine
{
public:
	OGLLine();
	OGLLine(vector<glm::vec3>* points);
	void setPoints(vector<glm::vec3>* points);
	void setPoints(vector<glm::vec3>* points, vector<GLuint>* indices);
	void setIndices(vector<GLuint>* indices);
	void setColor(glm::vec4 color);
	void setLineColor(glm::vec4 line_color);
	void setPointColor(glm::vec4 point_color);
	void setRenderMode(RenderMode render_mode);
	void setCurveMode(CurveMode curve_mode);
	void draw(const GLuint color_loc);

	void draw(RenderMode render_mode, CurveMode curve_mode, GLuint color_loc);

	vector<glm::vec3>* getPoints();
	GLfloat getTotalLength();
	//GLfloat getTotalLengthTill(int index);

	void clearLine();
	~OGLLine();


	void drawLines();
	void drawPoints();
private:
	RenderMode render_mode;
	CurveMode curve_mode;
	//vector<GLfloat>* points;
	vector<glm::vec3> points;
	vector<GLuint> indices;
	glm::vec4 line_color, point_color;
	GLuint VAO, VBO, EBO;
	
	void setUp();

};

