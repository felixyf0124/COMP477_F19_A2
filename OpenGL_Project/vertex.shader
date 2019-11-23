
#version 330 core
  
layout (location = 0) in vec3 _pos;

//out vec4 vCol;

uniform vec4 _color;
uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _projection;
out vec4 color_;
void main() 
{
	
	color_ = _color;
	//color_ = vec4(clamp(_pos, 0.0f, 1.0f), 1.0f);;
	
	gl_Position = _projection * _view * _model * vec4(_pos, 1.0);
	//gl_Position = vec4(_pos, 1.0);
	//vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
}