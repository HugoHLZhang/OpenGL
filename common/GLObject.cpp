#pragma once
#include "GLObject.h"
#include "../common/Struct.h"
#include "../common/VertexArray.h"
#include "../common/VertexBuffer.h"
#include "../common/BufferLayout.h"
#include "../common/Texture.h"
#include "../common/IndexBuffer.h"


#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glfw.h"
#include "../libs/imgui/imgui_impl_opengl3.h"
#include "../common/Matrix.h"
#include "../common/Vertex.h"

#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

GLObject::GLObject(const GLShader& glshader, const Vector3& position, const Vector3& rotation, const Vector3& scale) : mPos(position), mRot(rotation), mScale(scale), mName("")
{
	mGLShader = glshader;
}

GLObject::~GLObject()
{
	for (auto i : mVAs)
		delete i;
	mVAs.clear();
	for (auto i : mVBs)
		delete i;
	mVBs.clear();
	for (auto i : mText)
		delete i;
	mText.clear();
}

void GLObject::AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout) {

	VertexArray* va = new VertexArray();
	VertexBuffer* vb = new VertexBuffer(&vertices[0], sizeof(vertices[0]) * vertices.size());
	mCounts.push_back(vertices.size());
	va->AddBuffer(*vb, layout);
	mVAs.push_back(va);
	mVBs.push_back(vb);
	mMat.push_back({Vector3(0,0,0), Vector3(1,1,1), Vector3(1,1,1), 0});;
	mText.push_back(nullptr);
}

void GLObject::AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout, const Material& materials) {

	VertexArray* va = new VertexArray();
	VertexBuffer* vb = new VertexBuffer(&vertices[0], sizeof(vertices[0]) * vertices.size());
	mCounts.push_back(vertices.size());
	va->AddBuffer(*vb, layout);
	mVAs.push_back(va);
	mVBs.push_back(vb);
	mMat.push_back(materials);
	mText.push_back(nullptr);
}

void GLObject::AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout, const Material& materials, const std::string& texName) {
	
	VertexArray *va = new VertexArray();
	VertexBuffer *vb = new VertexBuffer(&vertices[0], sizeof(vertices[0]) * vertices.size());
	mCounts.push_back(vertices.size());
	va->AddBuffer(*vb, layout);
	mVAs.push_back(va);
	mVBs.push_back(vb);
	mMat.push_back(materials);
	mText.push_back(new Texture(texName));
}

void GLObject::Render(){

	auto program = mGLShader.GetProgram();
	glUseProgram(program);
	Matrix4x4 transform = GetTransform();
	glUniformMatrix4fv(glGetUniformLocation(program, "u_Model"), 1, GL_TRUE, transform.GetData());
	
	int KaLocation = glGetUniformLocation(program, "u_Material.Ka");
	int KdLocation = glGetUniformLocation(program, "u_Material.Kd");
	int KsLocation = glGetUniformLocation(program, "u_Material.Ks");
	int NsLocation = glGetUniformLocation(program, "u_Material.Ns");
	int hasTextureLocation = glGetUniformLocation(program, "u_HasTexture");
	int textureLocation = glGetUniformLocation(program, "u_Map_Kd");
	
	for (int i = 0; i < mVAs.size(); i++) {
		glUniform3fv(KaLocation, 1, &mMat[i].ambient[0]);
		glUniform3fv(KdLocation, 1, &mMat[i].diffuse[0]);
		glUniform3fv(KsLocation, 1, &mMat[i].specular[0]);
		glUniform1f(NsLocation, mMat[i].specularExponent);
		
		if (mText[i]) {
			mText[i]->Bind();
			glUniform1i(hasTextureLocation, 1);
			glUniform1i(textureLocation, mText[i]->GetSlot());
		}
		else {
			glUniform1i(hasTextureLocation, 0);
		}
		mVAs[i]->Bind();
		glDrawArrays(GL_TRIANGLES, 0, mCounts[i]);
	}
	glUseProgram(0);
}
