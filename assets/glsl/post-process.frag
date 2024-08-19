#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_textureCoordinates;
in flat uint v_textureIndex;

uniform sampler2D u_textures[32];

// -----------------------------------------

layout(std140, binding = 0) uniform Camera {
	mat4 u_projectionView;
	vec3 u_position;
};

// -----------------------------------------

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

const int MAX_DIRECTIONAL_LIGHTS = 32;
layout(std140, binding = 1) uniform DirectionalLights {
	DirectionalLight u_directionalLight[MAX_DIRECTIONAL_LIGHTS];
};

// -----------------------------------------

void main()
{
	float isObject = texture(u_textures[v_textureIndex + 1], v_textureCoordinates).a;
	if (isObject == 0.0f) {
		color = vec4(0,0,0,0);
		return;
	}

	vec3 albedo    = texture(u_textures[v_textureIndex + 0], v_textureCoordinates).rgb;
	float specular = texture(u_textures[v_textureIndex + 0], v_textureCoordinates).a;
	vec3 position  = texture(u_textures[v_textureIndex + 1], v_textureCoordinates).rgb;
	vec3 normal    = texture(u_textures[v_textureIndex + 2], v_textureCoordinates).rgb;


	vec3 lighting = vec3(0.0f, 0.0f, 0.0f);//albedo * v_color.xyz;
	vec3 viewDirection = normalize(u_position - position);

	// Loop through all directional lights
	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i) {
		// Diffuse
		vec3 lightDirection = normalize(-u_directionalLight[i].direction);
		float diffuse = max(dot(normal, lightDirection), 0.0f);

		// Specular
		vec3 reflectionDirection = reflect(-lightDirection, normal);
		float specular = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 32);

		lighting +=
			(albedo * u_directionalLight[i].ambient) +
			(albedo * diffuse * u_directionalLight[i].diffuse) +
			(specular * u_directionalLight[i].specular);
	}

	// Loop through all point lights
		// TODO
		// vec3 lightDirection = normalize(lightPosition - position);
		// float diffuse = max(dot(normal, lightDirection), 0.0f);
		// lighting += diffuse * albedo * u_lightColor;

	// Loop through all spot lights
		// TODO

	color = vec4(lighting, 1.0f);
}
