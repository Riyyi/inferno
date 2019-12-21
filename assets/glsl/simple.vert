#version 450 core

layout(location = 0) in vec3 a_position;

out vec3 position;

void main()
{
	position = a_position;
	gl_Position = vec4(a_position, 1.0f);
}
