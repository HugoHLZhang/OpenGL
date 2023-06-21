#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "VertexArray.h"

VertexArray::VertexArray() : mID(0)
{
	glGenVertexArrays(1, &mID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &mID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(mID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.cnt, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.cnt * BufferElement::GetSizeOfType(element.type);
	}
}