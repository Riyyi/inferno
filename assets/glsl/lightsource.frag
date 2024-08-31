#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec3 v_textureCoordinates;
in flat uint v_textureIndex;

uniform samplerCube u_textures[32];

void main()
{
	// Prevent u_textures variable from getting optimized away
	if (texture(u_textures[0], vec3(0.0f)).x > 99999.0f) { discard; }

	color = v_color;
}
