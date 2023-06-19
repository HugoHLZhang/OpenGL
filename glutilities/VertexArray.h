#pragma once
#include "VertexBuffer.h"
#include "BufferLayout.h"
class VertexArray {
	
private:
	unsigned int m_ID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const BufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};