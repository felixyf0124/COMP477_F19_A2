#include "OGLShader.h"

OGLShader::OGLShader():ID(0)
{
	//ID = 0;
}

void OGLShader::createFromStrSrc(const char* vertex_code, const char* fragment_code)
{
	compile(vertex_code, fragment_code);
}

void OGLShader::createFromFileSrc(const char* vertex_loc, const char* fragment_loc)
{
	string _vertex_code_str = readFile(vertex_loc);
	string _fragment_code_str = readFile(fragment_loc);
	const char* _vertex_code = _vertex_code_str.c_str();
	const char* _fragment_code = _fragment_code_str.c_str();
	
	createFromStrSrc(_vertex_code, _fragment_code);
	//compile(_vertex_code, _fragment_code);
}

string OGLShader::readFile(const char* file_loc)
{
	string _content="";
	ifstream _stream(file_loc, ios::in);

	if (_stream.is_open()) {
		string _line = "";
		while (getline(_stream, _line))
			_content.append(_line + "\n");
		_stream.close();
	}
	else {
		printf("Failed to open %s. Are you in the right directory ?\n", file_loc);
		getchar();
		exit(-1);
	}
	return _content;
}

void OGLShader::useShader()
{
	glUseProgram(ID);
}

void OGLShader::clearShader()
{
	if (ID != 0) {
		glDeleteProgram(ID);
		ID = 0;
	}

	model_loc = 0;
	//view_loc = 0;
	projection_loc = 0;
	color_loc = 0;
}

GLuint OGLShader::getModelLoc()
{
	return model_loc;
}

GLuint OGLShader::getViewLoc()
{
	return view_loc;
}

GLuint OGLShader::getProjectionLoc()
{
	return projection_loc;
}

GLuint OGLShader::getColorLoc()
{
	return color_loc;
}

OGLShader::~OGLShader()
{
	clearShader();
}

void OGLShader::compile(const char* vertex_code, const char* fragment_code)
{
	ID = glCreateProgram();

	if (!ID) {
		printf("Error creating shader program!\n");
		return;
	}
	
	addShader(ID, vertex_code, GL_VERTEX_SHADER);
	addShader(ID, fragment_code, GL_FRAGMENT_SHADER);

	GLint _result = 0;
	GLchar _log[1024] = { 0 };

	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &_result);
	if (!_result) {
		glGetProgramInfoLog(ID, sizeof(_log), NULL, _log);
		printf("Error linking program: '%s'\n", _log);
		return;
	}

	glValidateProgram(ID);
	glGetProgramiv(ID, GL_VALIDATE_STATUS, &_result);
	if (!_result)
	{
		glGetProgramInfoLog(ID, sizeof(_log), NULL, _log);
		printf("Error validating program: '%s'\n", _log);
		return;
	}


	projection_loc = glGetUniformLocation(ID, "_projection");
	view_loc = glGetUniformLocation(ID, "_view");
	model_loc = glGetUniformLocation(ID, "_model");
	color_loc = glGetUniformLocation(ID, "_color");
}

void OGLShader::addShader(GLuint program, const char* shader_code, GLenum shader_type)
{
	GLuint _shader = glCreateShader(shader_type);
	
	const GLchar* _shader_code[1];
	_shader_code[0] = shader_code;
	
	GLint _code_length[1];
	_code_length [0] = strlen(shader_code);
	
	glShaderSource(_shader, 1, _shader_code, _code_length);
	glCompileShader(_shader);

	GLint _result = 0;
	GLchar _log[1024] = { 0 };

	glGetShaderiv(_shader, GL_COMPILE_STATUS, &_result);
	if (!_result)
	{
		glGetShaderInfoLog(_shader, sizeof(_log), NULL, _log);
		printf("Error compiling the %d shader: '%s'\n", shader_type, _log);
		return;
	}

	glAttachShader(program, _shader);
}
