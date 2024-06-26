#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_textureCoordinates;
layout(location = 3) in float a_textureIndex;

out vec3 v_normal;
out vec2 v_textureCoordinates;
out flat float v_textureIndex;

uniform mat4 u_projectionView;

void main()
{
	v_normal = a_normal;
	v_textureCoordinates = a_textureCoordinates;
	v_textureIndex = a_textureIndex;
	// Vclip = Camera projection * Camera view * Model transform * Vlocal
	gl_Position = u_projectionView * vec4(a_position, 1.0f);
}
