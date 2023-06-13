#version 120

// variable predefinie 
// en sortie: gl_FragColor
// elle est de type vec4

uniform float u_Time; // maj par glUniform1f

varying vec4 v_Color;

void main(void)
{
    // x €[-1;+1] -> [0;1]
    // (x + 1)/2 <=> x * 1/2 + 1/2 
    gl_FragColor = vec4(sin(u_Time) * 0.5 + 0.5, vec3(0.0))  
                   + v_Color;
}