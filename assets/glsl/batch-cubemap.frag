#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec3 v_textureCoordinates;
in flat uint v_textureIndex;

uniform samplerCube u_textures[32];

void main()
{
	vec4 textureColor = v_color;
	switch(v_textureIndex) {
		case 0:  break; // Texture unit 0 is reserved for no texture
		case 1:  textureColor *= texture(u_textures[1],  v_textureCoordinates); break;
		case 2:  textureColor *= texture(u_textures[2],  v_textureCoordinates); break;
		case 3:  textureColor *= texture(u_textures[3],  v_textureCoordinates); break;
		case 4:  textureColor *= texture(u_textures[4],  v_textureCoordinates); break;
		case 5:  textureColor *= texture(u_textures[5],  v_textureCoordinates); break;
		case 6:  textureColor *= texture(u_textures[6],  v_textureCoordinates); break;
		case 7:  textureColor *= texture(u_textures[7],  v_textureCoordinates); break;
		case 8:  textureColor *= texture(u_textures[8],  v_textureCoordinates); break;
		case 9:  textureColor *= texture(u_textures[9],  v_textureCoordinates); break;
		case 10: textureColor *= texture(u_textures[10], v_textureCoordinates); break;
		case 11: textureColor *= texture(u_textures[11], v_textureCoordinates); break;
		case 12: textureColor *= texture(u_textures[12], v_textureCoordinates); break;
		case 13: textureColor *= texture(u_textures[13], v_textureCoordinates); break;
		case 14: textureColor *= texture(u_textures[14], v_textureCoordinates); break;
		case 15: textureColor *= texture(u_textures[15], v_textureCoordinates); break;
		case 16: textureColor *= texture(u_textures[16], v_textureCoordinates); break;
		case 17: textureColor *= texture(u_textures[17], v_textureCoordinates); break;
		case 18: textureColor *= texture(u_textures[18], v_textureCoordinates); break;
		case 19: textureColor *= texture(u_textures[19], v_textureCoordinates); break;
		case 20: textureColor *= texture(u_textures[20], v_textureCoordinates); break;
		case 21: textureColor *= texture(u_textures[21], v_textureCoordinates); break;
		case 22: textureColor *= texture(u_textures[22], v_textureCoordinates); break;
		case 23: textureColor *= texture(u_textures[23], v_textureCoordinates); break;
		case 24: textureColor *= texture(u_textures[24], v_textureCoordinates); break;
		case 25: textureColor *= texture(u_textures[25], v_textureCoordinates); break;
		case 26: textureColor *= texture(u_textures[26], v_textureCoordinates); break;
		case 27: textureColor *= texture(u_textures[27], v_textureCoordinates); break;
		case 28: textureColor *= texture(u_textures[28], v_textureCoordinates); break;
		case 29: textureColor *= texture(u_textures[29], v_textureCoordinates); break;
		case 30: textureColor *= texture(u_textures[30], v_textureCoordinates); break;
		case 31: textureColor *= texture(u_textures[31], v_textureCoordinates); break;
	}
	color = textureColor;
}
