#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_textureCoordinates;
layout(location = 3) in uint a_textureIndex;

out vec4 v_color;
out vec2 v_textureCoordinates;
out flat uint v_textureIndex;

void main()
{
	v_color = a_color;
	v_textureCoordinates = a_textureCoordinates;
	v_textureIndex = a_textureIndex;
	// Vclip = Model transform * Vlocal
	gl_Position = vec4(a_position, 1.0f);
}
