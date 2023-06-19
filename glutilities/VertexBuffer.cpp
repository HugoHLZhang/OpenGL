#pragma once
#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "VertexBuffer.h"
VertexBuffer::VertexBuffer(const void* data, const unsigned int& size) : m_ID(0)
{
	glGenBuffers(1,&m_ID);
	glBindBuffer(GL_ARRAY_BUFFER,m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1,&m_ID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER,m_ID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
