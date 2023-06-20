#pragma once
#include "GLObject.h"
#include "../common/Struct.h"
#include "../glutilities/VertexArray.h"
#include "../glutilities/VertexBuffer.h"
#include "../glutilities/BufferLayout.h"
#include "../glutilities/Texture.h"
#include "../glutilities/IndexBuffer.h"


#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glfw.h"
#include "../libs/imgui/imgui_impl_opengl3.h"
#include "../common/Matrix.h"
#include "../common/Vector.h"

#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

GLObject::GLObject(const GLShader& glshader, const Vector3& position, const Vector3& rotation, const Vector3& scale) : m_Position(position), m_Rotation(rotation), m_Scale(scale), m_Name("")
{
	m_GLShader = glshader;
}


GLObject::~GLObject()
{
	for (auto i : m_VAs)
		delete i;
	m_VAs.clear();
	for (auto i : m_VBs)
		delete i;
	m_VBs.clear();
	for (auto i : m_Textures)
		delete i;
	m_Textures.clear();
}

void GLObject::AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout) {

	VertexArray* va = new VertexArray();
	VertexBuffer* vb = new VertexBuffer(&vertices[0], sizeof(vertices[0]) * vertices.size());
	m_Counts.push_back(vertices.size());
	va->AddBuffer(*vb, layout);
	m_VAs.push_back(va);
	m_VBs.push_back(vb);
	m_Materials.push_back({Vector3(0,0,0), Vector3(1,1,1), Vector3(1,1,1), 0});;
	m_Textures.push_back(nullptr);
}

void GLObject::AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout, const Material& materials) {

	VertexArray* va = new VertexArray();
	VertexBuffer* vb = new VertexBuffer(&vertices[0], sizeof(vertices[0]) * vertices.size());
	m_Counts.push_back(vertices.size());
	va->AddBuffer(*vb, layout);
	m_VAs.push_back(va);
	m_VBs.push_back(vb);
	m_Materials.push_back(materials);
	m_Textures.push_back(nullptr);
}

void GLObject::AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout, const Material& materials, const std::string& textureName) {
	
	VertexArray *va = new VertexArray();
	VertexBuffer *vb = new VertexBuffer(&vertices[0], sizeof(vertices[0]) * vertices.size());
	m_Counts.push_back(vertices.size());
	va->AddBuffer(*vb, layout);
	m_VAs.push_back(va);
	m_VBs.push_back(vb);
	m_Materials.push_back(materials);
	m_Textures.push_back(new Texture(textureName));
}

void GLObject::Render(){

	auto program = m_GLShader.GetProgram();
	glUseProgram(program);
	Matrix4x4 transform = GetTransform();
	glUniformMatrix4fv(glGetUniformLocation(program, "u_Model"), 1, GL_TRUE, transform.GetData());
	
	int KaLocation = glGetUniformLocation(program, "u_Material.Ka");
	int KdLocation = glGetUniformLocation(program, "u_Material.Kd");
	int KsLocation = glGetUniformLocation(program, "u_Material.Ks");
	int NsLocation = glGetUniformLocation(program, "u_Material.Ns");
	int hasTextureLocation = glGetUniformLocation(program, "u_HasTexture");
	int textureLocation = glGetUniformLocation(program, "u_Map_Kd");
	
	for (int i = 0; i < m_VAs.size(); i++) {
		glUniform3fv(KaLocation, 1, &m_Materials[i].ambient[0]);
		glUniform3fv(KdLocation, 1, &m_Materials[i].diffuse[0]);
		glUniform3fv(KsLocation, 1, &m_Materials[i].specular[0]);
		glUniform1f(NsLocation, m_Materials[i].specularExponent);
		
		if (m_Textures[i]) {
			m_Textures[i]->Bind();
			glUniform1i(hasTextureLocation, 1);
			glUniform1i(textureLocation, m_Textures[i]->GetSlot());
		}
		else {
			glUniform1i(hasTextureLocation, 0);
		}
		m_VAs[i]->Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_Counts[i]);
	}
	glUseProgram(0);
}