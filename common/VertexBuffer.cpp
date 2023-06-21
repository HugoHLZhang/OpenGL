#pragma once
#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "VertexBuffer.h"
VertexBuffer::VertexBuffer(const void* data, const unsigned int& size) : mID(0)
{
	glGenBuffers(1,&mID);
	glBindBuffer(GL_ARRAY_BUFFER,mID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1,&mID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER,mID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
