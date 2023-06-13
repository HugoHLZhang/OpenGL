#version 120

// role du Vertex Shader:
// produire (au minimum)
// une position

// les entrees (inputs):
// le mot cle 'attribute' 
// indique que la donnee
// provient du C++

attribute vec2 a_Position;
attribute vec3 a_Color;

uniform float u_Time; // maj par glUniform1f

varying vec4 v_Color;

void main(void)
{
	v_Color = vec4(
	    a_Color, 1.0);

	vec2 movement = vec2(cos(u_Time), sin(u_Time));

	gl_Position = 
	vec4(a_Position.xy + movement, 0.0, 1.0);
}

// gl_Position est une var.
// predefinie de type vec4
// avec le mot cle 'varying'
