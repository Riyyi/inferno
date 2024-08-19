#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in uint a_textureIndex;

out vec4 v_color;
out vec3 v_textureCoordinates;
out flat uint v_textureIndex;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_projectionView;
	vec3 u_position;
};

void main()
{
	v_color = a_color;
	v_textureCoordinates = a_position;
	v_textureIndex = a_textureIndex;
	// Vclip = Camera projection * Camera view * Model transform * Vlocal
	gl_Position = u_projectionView * vec4(a_position, 1.0f);
}
