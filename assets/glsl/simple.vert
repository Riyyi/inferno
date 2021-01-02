#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

out vec4 v_color;

void main()
{
	v_color = a_color;
	gl_Position = vec4(a_position, 1.0f);
}
