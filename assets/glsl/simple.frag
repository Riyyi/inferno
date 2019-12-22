#version 450 core

layout(location = 0) out vec4 color;

in vec3 p_position;
in vec4 p_color;

void main()
{
	color = p_color;
}
