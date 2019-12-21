#version 450 core

layout(location = 0) out vec4 color;

in vec3 position;

void main()
{
	color = vec4(position * 0.5f + 0.5f, 1.0f);
}
