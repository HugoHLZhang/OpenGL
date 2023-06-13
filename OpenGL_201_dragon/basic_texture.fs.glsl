#version 120

// variable predefinie 
// en sortie: gl_FragColor
// elle est de type vec4

uniform float u_Time; // maj par glUniform1f

uniform sampler2D u_Sampler;

varying vec2 v_TexCoords;
varying vec4 v_Color;

void main(void)
{
    // x €[-1;+1] -> [0;1]
    // (x + 1)/2 <=> x * 1/2 + 1/2 
    gl_FragColor = v_Color * texture2D(u_Sampler, v_TexCoords);
}