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
uniform bool u_HasTexture;
uniform vec3 u_SkyDir;
uniform vec3 u_SkyColor;
uniform vec3 u_GroundColor;

float f_diffuse(vec3 normal, vec3 lightDirection)
{
	return max(dot(normal, lightDirection), 0.0);
}

void main(void) {

	vec3 normal = normalize(v_normal);

	vec3 skyDir = -normalize(u_SkyDir);

	float NdotSky = max(dot(normal, skyDir), 0.0);

	float HemisphereFactor = NdotSky * 0.5 + 0.5;

	vec3 lightColor = vec3(u_lightColor);

	vec3 light = u_lightIntensity * u_lightColor;

	vec3 lightDirection = vec3(u_lightDirection);

	vec3 text_Kd = u_HasTexture ? vec3(texture(u_Map_Kd, v_textcoords)) : vec3(0.2,0.2,0.2);

	float NdotL = f_diffuse(normalize(normal), normalize(-lightDirection));

	vec3 diffuse = NdotL * u_Material.Kd * text_Kd;

	vec3 ambient = u_Material.Ka * text_Kd * mix(u_SkyColor, u_GroundColor, HemisphereFactor);
	
	vec3 specular = u_Material.Ks * pow(max(dot(normalize(-lightDirection), normalize(normal)), 0.0), u_Material.Ns);

    vec3 color = light * (diffuse + ambient + specular);

	o_fragColor = vec4(color, 1.0);
}

