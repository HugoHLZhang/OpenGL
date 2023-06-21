#version 330

in vec2 v_textcoords;
in vec3 v_normal;

struct Material
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Ns;
};

out vec4 o_fragColor;

layout (shared) uniform Lights
{
	uniform vec4 u_lightDirection;
	uniform vec3 u_lightColor;
	uniform float u_lightIntensity;
};

uniform Material u_Material;
uniform sampler2D u_Map_Kd;
uniform bool hasTexture;

float f_diffuse(vec3 normal, vec3 lightDirection)
{
	return max(dot(normal, lightDirection), 0.0);
}

void main(void) {

	vec3 normal = normalize(v_normal);

	vec3 lightColor = vec3(u_lightColor);

	vec3 light = u_lightIntensity * u_lightColor;

	vec3 lightDirection = vec3(u_lightDirection);

	vec3 text_Kd = hasTexture ? vec3(texture(u_Map_Kd, v_textcoords)) : vec3(0.2,0.2,0.2);

	float NdotL = f_diffuse(normalize(normal), normalize(-lightDirection));

	vec3 diffuse = NdotL * u_Material.Kd * text_Kd;

	vec3 ambient = u_Material.Ka * text_Kd;

    vec3 color = light * (diffuse + ambient);

	o_fragColor = vec4(color, 1.0);
}

