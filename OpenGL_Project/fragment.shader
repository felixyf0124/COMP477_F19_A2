#version 330 core

out vec4 color;
in vec4 color_;
void main()
{
	color = color_;
} 