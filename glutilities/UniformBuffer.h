#pragma once
#include "BufferLayout.h"
#include <string>
class UniformBuffer {
	
private:
	unsigned int m_ID;
	inline static unsigned int m_BindPointCounter = 0;
	unsigned int m_BindPoint;
	unsigned int m_Size;
	std::string m_Name;
	BufferLayout m_Layout;
public:
	UniformBuffer(const std::string& name, const void* data, const unsigned int& size);
	~UniformBuffer();

	void Bind() const;
	void Unbind() const;
	
	void AddShader(const unsigned int& shaderID);

	void AddData(const void* data, const BufferLayout& layout, const bool& update = false);
	void AddDataf(const void* data, const unsigned int& count, const bool& update = false);
	BufferLayout GetLayout() const { return m_Layout; }
};

//unsigned int UniformBuffer::m_BindPointCounter = 0;