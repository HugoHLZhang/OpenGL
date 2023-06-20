#pragma once
#include <vector>
#include <GL/glew.h>

struct BufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		return 0;
	}
};

class BufferLayout {
private: 
	std::vector<BufferElement> m_Elements;
	unsigned int m_Stride;
	
public:
	BufferLayout() : m_Stride(0) {}
	~BufferLayout() {};

	template<typename T>
	void Push(const unsigned int& count) {};

	template<>
	void Push<float>(const unsigned int& count){
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * BufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(const unsigned int& count){
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * BufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}
	
	template<>
	void Push<unsigned char>(const unsigned int& count){
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * BufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
	

	void Push(const BufferLayout& layout) {
		for (BufferElement elem : layout.GetElements()) {
			m_Elements.push_back(elem);
			m_Stride += elem.count * BufferElement::GetSizeOfType(elem.type);
		}
	}

	inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};