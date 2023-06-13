#version 120

// variable predefinie 
// en sortie: gl_FragColor
// elle est de type vec4

uniform float u_Time; // maj par glUniform1f

uniform sampler2D u_Sampler;

varying vec2 v_TexCoords;
varying vec3 v_Normal;

uniform vec3 u_LightDirection; // direction VERS la lumiere

// pas obligatoire
const vec3 diffuseLightColor = vec3(1.0, 0.0, 0.0);

float diffuse(vec3 N, vec3 L)
{
    float lambert = max(dot(N, L), 0.0);
    return lambert;
}

void main(void)
{
    // il est tres important de renormaliser les normales
    // interpolees depuis le Vertex Shader
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(u_LightDirection); // pas utile en pratique ici

    vec4 texColor = texture2D(u_Sampler, v_TexCoords);

    vec3 diffuseColor = diffuse(N, L) * diffuseLightColor * texColor.rgb;

    gl_FragColor = vec4(diffuseColor, texColor.a);
}