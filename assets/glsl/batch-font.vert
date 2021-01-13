#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_textureCoordinates;
layout(location = 3) in float a_textureIndex;
layout(location = 4) in float a_width;
layout(location = 5) in float a_edge;
layout(location = 6) in float a_borderWidth;
layout(location = 7) in float a_borderEdge;
layout(location = 8) in vec4 a_borderColor;
layout(location = 9) in float a_offset;

out vec4 v_color;
out vec2 v_textureCoordinates;
out flat float v_textureIndex;
out float v_width;
out float v_edge;
out float v_borderWidth;
out float v_borderEdge;
out vec4 v_borderColor;
out flat float v_offset;

void main()
{
	v_color = a_color;
	v_textureCoordinates = a_textureCoordinates;
	v_textureIndex = a_textureIndex;
	v_width = a_width;
	v_edge = a_edge;
	v_borderWidth = a_borderWidth;
	v_borderEdge = a_borderEdge;
	v_borderColor = a_borderColor;
	v_offset = a_offset;
	gl_Position = vec4(a_position, 1.0f);
}
