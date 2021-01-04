#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture;

void main()
{
	color = v_color * texture(u_texture, v_texCoord);
}
