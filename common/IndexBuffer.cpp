#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int& size) : mID(0), mCount(size)
{
	glGenBuffers(1, &mID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &mID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mID);
}


void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

