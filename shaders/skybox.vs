#version 330 core
layout (location = 0) in vec3 a_pos;

out vec3 TexCoords;

//uniform mat4 u_Projection;
//uniform mat4 u_View;

layout (std140) uniform Matrices
{
	mat4 u_View;
    mat4 u_Projection;
};

void main()
{
    TexCoords = a_pos;
    vec4 pos = u_Projection * mat4(mat3(u_View)) * vec4(a_pos, 1.0);
    gl_Position = pos.xyww;

}  

