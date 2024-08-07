#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_textureCoordinates;
in flat uint v_textureIndex;
in float v_width;
in float v_edge;
in float v_borderWidth;
in float v_borderEdge;
in vec4 v_borderColor;
in flat float v_offset;

uniform sampler2D u_textures[32];

float alpha(float textureAlpha)
{
	float distance = 1.0f - textureAlpha;
	float alpha    = 1.0f - smoothstep(v_width, v_width + v_edge, distance);

	return alpha;
}

void main()
{
	vec4 textureColor = v_color;
	switch(v_textureIndex) {
		case 0:  break; // Texture unit 0 is reserved for no texture
		// case 1: textureColor.a = 1; break;
		case 1:  textureColor.a = alpha(texture(u_textures[1], v_textureCoordinates).a); break;
		// case 1:  textureColor *= texture(u_textures[1],  v_textureCoordinates); break;
		case 2:  textureColor.a = alpha(texture(u_textures[2],  v_textureCoordinates).a); break;
		case 3:  textureColor.a = alpha(texture(u_textures[3],  v_textureCoordinates).a); break;
		case 4:  textureColor.a = alpha(texture(u_textures[4],  v_textureCoordinates).a); break;
		case 5:  textureColor.a = alpha(texture(u_textures[5],  v_textureCoordinates).a); break;
		case 6:  textureColor.a = alpha(texture(u_textures[6],  v_textureCoordinates).a); break;
		case 7:  textureColor.a = alpha(texture(u_textures[7],  v_textureCoordinates).a); break;
		case 8:  textureColor.a = alpha(texture(u_textures[8],  v_textureCoordinates).a); break;
		case 9:  textureColor.a = alpha(texture(u_textures[9],  v_textureCoordinates).a); break;
		case 10: textureColor.a = alpha(texture(u_textures[10], v_textureCoordinates).a); break;
		case 11: textureColor.a = alpha(texture(u_textures[11], v_textureCoordinates).a); break;
		case 12: textureColor.a = alpha(texture(u_textures[12], v_textureCoordinates).a); break;
		case 13: textureColor.a = alpha(texture(u_textures[13], v_textureCoordinates).a); break;
		case 14: textureColor.a = alpha(texture(u_textures[14], v_textureCoordinates).a); break;
		case 15: textureColor.a = alpha(texture(u_textures[15], v_textureCoordinates).a); break;
		case 16: textureColor.a = alpha(texture(u_textures[16], v_textureCoordinates).a); break;
		case 17: textureColor.a = alpha(texture(u_textures[17], v_textureCoordinates).a); break;
		case 18: textureColor.a = alpha(texture(u_textures[18], v_textureCoordinates).a); break;
		case 19: textureColor.a = alpha(texture(u_textures[19], v_textureCoordinates).a); break;
		case 20: textureColor.a = alpha(texture(u_textures[20], v_textureCoordinates).a); break;
		case 21: textureColor.a = alpha(texture(u_textures[21], v_textureCoordinates).a); break;
		case 22: textureColor.a = alpha(texture(u_textures[22], v_textureCoordinates).a); break;
		case 23: textureColor.a = alpha(texture(u_textures[23], v_textureCoordinates).a); break;
		case 24: textureColor.a = alpha(texture(u_textures[24], v_textureCoordinates).a); break;
		case 25: textureColor.a = alpha(texture(u_textures[25], v_textureCoordinates).a); break;
		case 26: textureColor.a = alpha(texture(u_textures[26], v_textureCoordinates).a); break;
		case 27: textureColor.a = alpha(texture(u_textures[27], v_textureCoordinates).a); break;
		case 28: textureColor.a = alpha(texture(u_textures[28], v_textureCoordinates).a); break;
		case 29: textureColor.a = alpha(texture(u_textures[29], v_textureCoordinates).a); break;
		case 30: textureColor.a = alpha(texture(u_textures[30], v_textureCoordinates).a); break;
		case 31: textureColor.a = alpha(texture(u_textures[31], v_textureCoordinates).a); break;
	}
	// textureColor.a = 1; // tmp
	color = textureColor;
}
