#pragma once 
#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "UniformBuffer.h"
#include <string>
#include "BufferLayout.h"
#include <iostream>
static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);

}

static void GLCheckError() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
	}
}

UniformBuffer::UniformBuffer(const std::string& name, const void* data, const unsigned int& size) : mID(0), mBindPoint(mBindPointCnt++), mSize(size), mName(name)
{

	glGenBuffers(1, &mID);
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, mBindPoint, mID, 0, size);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1,&mID);
}

void UniformBuffer::Bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER,mID);
}

void UniformBuffer::Unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER,0);
}

void UniformBuffer::AddShader(const unsigned int& shaderID) {
	const char* name = mName.c_str();
	int i = glGetUniformBlockIndex(shaderID, name);
	glUniformBlockBinding(shaderID, i, mBindPoint);
}

void UniformBuffer::AddData(const void* data, const BufferLayout& layout, const bool& update)
{
	if (update)
		mLayout = BufferLayout();
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferSubData(GL_UNIFORM_BUFFER, mLayout.GetStride(), layout.GetStride(), data);
	mLayout.Push(layout);
}

void UniformBuffer::AddDataf(const void* data, const unsigned int& cnt, const bool& update)
{
	if (update)
		mLayout = BufferLayout();
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferSubData(GL_UNIFORM_BUFFER, mLayout.GetStride(), cnt * sizeof(float), data);
	mLayout.Push<float>(cnt);
}
