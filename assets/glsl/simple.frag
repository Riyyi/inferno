#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;

void main()
{
	color = v_color;
}
