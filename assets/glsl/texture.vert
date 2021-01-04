#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;

out vec4 v_color;
out vec2 v_texCoord;

uniform mat4 u_projectionView;
uniform mat4 u_model;

void main()
{
	v_color = a_color;
	v_texCoord = a_texCoord;
	// Vclip = Camera projection * Camera view * Model transform * Vlocal
	gl_Position = u_projectionView * u_model * vec4(a_position, 1.0f);
}
