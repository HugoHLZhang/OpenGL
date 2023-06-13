#version 120

// role du Vertex Shader:
// produire (au minimum)
// une position

// les entrees (inputs):
// le mot cle 'attribute' 
// indique que la donnee
// provient du C++

attribute vec3 a_Position;
attribute vec3 a_Color;
attribute vec2 a_TexCoords;

uniform float u_Time; // maj par glUniform1f

uniform mat4 u_RotationMatrix;
uniform mat4 u_TranslationMatrix;
uniform mat4 u_ProjectionMatrix;

varying vec2 v_TexCoords;
varying vec4 v_Color;

void main(void)
{
	v_Color = vec4(a_Color, 1.0);

	v_TexCoords = vec2(a_TexCoords.x, 1.0 - a_TexCoords.y);

	vec4 position = vec4(a_Position, 1.0);

	gl_Position = u_ProjectionMatrix
				* u_TranslationMatrix
				* u_RotationMatrix
				* position;
}

// gl_Position est une var.
// predefinie de type vec4
// avec le mot cle 'varying'
