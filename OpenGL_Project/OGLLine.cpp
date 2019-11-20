#include "OGLLine.h"

OGLLine::OGLLine() : render_mode(LINE),
curve_mode(LINEAR),  line_color(glm::vec4(1.0f)),
point_color(glm::vec4(0.1f, 0.9f, 0.8f, 1.0f)), VAO(0), VBO(0), EBO(0)
{
	
}

OGLLine::OGLLine(vector<glm::vec3>* points) : render_mode(LINE),
curve_mode(LINEAR), line_color(glm::vec4(1.0f)),
point_color(glm::vec4(0.1f, 0.9f, 0.8f, 1.0f)), VAO(0), VBO(0), EBO(0)
{
	setPoints(points);
}

void OGLLine::setPoints(vector<glm::vec3>* points)
{
	//OGLLine();
	//clearLine();
	indices.clear();
	vector<GLfloat> _points;
	for (unsigned int i = 0; i < points->size(); ++i)
	{

		indices.push_back(i);
	}
	this->points = *points;
	setUp();
}

void OGLLine::setColor(glm::vec4 color)
{
	setLineColor(color);
	setPointColor(color);
}

void OGLLine::setLineColor(glm::vec4 line_color)
{
	this->line_color = line_color;
}

void OGLLine::setPointColor(glm::vec4 point_color)
{
	this->point_color = point_color;
}

void OGLLine::setRenderMode(RenderMode render_mode)
{
	this->render_mode = render_mode;
}

void OGLLine::setCurveMode(CurveMode curve_mode)
{
	this->curve_mode = curve_mode;
}

void OGLLine::draw(const GLuint color_loc)
{

	if (render_mode == LINE)
	{
		glUniform4fv(color_loc, 1, glm::value_ptr(line_color));
		drawLines();
	}
	if (render_mode == POINT)
	{
		glUniform4fv(color_loc, 1, glm::value_ptr(point_color));
		drawPoints();
	}
	if (render_mode == ALL || render_mode == POINT_AND_LINE)
	{
		glUniform4fv(color_loc, 1, glm::value_ptr(line_color));
		this->drawLines();
		glUniform4fv(color_loc, 1, glm::value_ptr(point_color));
		this->drawPoints();

	}
}



void OGLLine::draw(RenderMode render_mode, CurveMode curve_mode, GLuint color_loc)
{
	setRenderMode(render_mode);
	setCurveMode(curve_mode);
	draw(color_loc);
}

vector<glm::vec3>* OGLLine::getPoints()
{
	return &points;
}

GLfloat OGLLine::getTotalLength()
{
	GLfloat _length = 0.0f;
	for (unsigned int i = 0; i < points.size()-1; ++i)
	{
		const glm::vec3 _vector = points[i] - points[i + 1];
		_length += glm::length(_vector);
	}
	return GLfloat(_length);
}

//GLfloat OGLLine::getTotalLengthTill(int index)
//{
//	GLfloat _length = 0.0f;
//	for (unsigned int i = 0; i < index - 1; ++i)
//	{
//		const glm::vec3 _vector = points[i] - points[i + 1];
//		_length += glm::length(_vector);
//	}
//	return GLfloat();
//}

void OGLLine::clearLine()
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
}

OGLLine::~OGLLine()
{
	clearLine();
}

void OGLLine::setUp()
{
	//vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vbo
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
	
	//ebo
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void OGLLine::drawLines()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);

	glLineWidth(2.0f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OGLLine::drawPoints()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
	glPointSize(6.0f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


