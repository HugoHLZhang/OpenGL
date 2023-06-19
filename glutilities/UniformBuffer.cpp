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

UniformBuffer::UniformBuffer(const std::string& name, const void* data, const unsigned int& size) : m_ID(0), m_BindPoint(m_BindPointCounter++), m_Size(size), m_Name(name)
{

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, m_BindPoint, m_ID, 0, size);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1,&m_ID);
}

void UniformBuffer::Bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER,m_ID);
}

void UniformBuffer::Unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER,0);
}

void UniformBuffer::AddShader(const unsigned int& shaderID) {
	const char* name = m_Name.c_str();
	int i = glGetUniformBlockIndex(shaderID, name);
	glUniformBlockBinding(shaderID, i, m_BindPoint);
}

void UniformBuffer::AddData(const void* data, const BufferLayout& layout, const bool& update)
{
	if (update)
		m_Layout = BufferLayout();
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferSubData(GL_UNIFORM_BUFFER, m_Layout.GetStride(), layout.GetStride(), data);
	m_Layout.Push(layout);
}

void UniformBuffer::AddDataf(const void* data, const unsigned int& count, const bool& update)
{
	if (update)
		m_Layout = BufferLayout();
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferSubData(GL_UNIFORM_BUFFER, m_Layout.GetStride(), count * sizeof(float), data);
	m_Layout.Push<float>(count);
}
