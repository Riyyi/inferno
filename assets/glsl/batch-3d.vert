#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec2 a_textureCoordinates;
layout(location = 4) in uint a_textureIndex;

out vec3 v_normal;
out vec4 v_color;
out vec2 v_textureCoordinates;
out flat uint v_textureIndex;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_projectionView;
};

void main()
{
	v_normal = a_normal;
	v_color = a_color;
	v_textureCoordinates = a_textureCoordinates;
	v_textureIndex = a_textureIndex;
	// Vclip = Camera projection * Camera view * Model transform * Vlocal
	gl_Position = u_projectionView * vec4(a_position, 1.0f);
}
