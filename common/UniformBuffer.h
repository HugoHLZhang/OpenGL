#pragma once
#include "BufferLayout.h"
#include <string>
class UniformBuffer {
	
private:
	unsigned int mID;
	inline static unsigned int mBindPointCnt = 0;
	unsigned int mBindPoint;
	unsigned int mSize;
	std::string mName;
	BufferLayout mLayout;
public:
	UniformBuffer(const std::string& name, const void* data, const unsigned int& size);
	~UniformBuffer();

	void Bind() const;
	void Unbind() const;
	
	void AddShader(const unsigned int& shaderID);

	void AddData(const void* data, const BufferLayout& layout, const bool& update = false);
	void AddDataf(const void* data, const unsigned int& cnt, const bool& update = false);
	BufferLayout GetLayout() const { return mLayout; }
};

//unsigned int UniformBuffer::mBindPointCnt = 0;