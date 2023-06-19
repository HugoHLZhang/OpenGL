#version 330

layout(location=0) in vec3 a_position;
layout(location=1) in vec2 a_texcoords;
layout(location=2) in vec3 a_Normal;

out vec2 v_textcoords;
out vec3 v_normal;

layout (std140) uniform Matrices
{
	mat4 u_View;
    mat4 u_Projection;
};

uniform mat4 u_Model;
uniform mat4 u_Normal;

void main(void) 
{
	v_textcoords = a_texcoords;

	gl_Position = u_Projection * u_View * u_Model * vec4(a_position, 1.0);
	
	v_normal = mat3(transpose(inverse(u_Model))) * a_Normal;
}