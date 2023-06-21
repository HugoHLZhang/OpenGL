#pragma once
#include <vector>
#include <GL/glew.h>

struct BufferElement
{
	unsigned int type;
	unsigned int cnt;
	unsigned char normalized;
	
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			
				return 4;
			case GL_UNSIGNED_INT:	
				return 4;
			case GL_UNSIGNED_BYTE:	
				return 1;
			default :
				return 0;
		}
	}
};

class BufferLayout {
private: 
	std::vector<BufferElement> mElements;
	unsigned int mStride;
	
public:
	BufferLayout() : mStride(0) {}
	~BufferLayout() {};

	template<typename T>
	void Push(const unsigned int& cnt) {};

	template<>
	void Push<float>(const unsigned int& cnt){
		mElements.push_back({ GL_FLOAT, cnt, GL_FALSE });
		mStride += cnt * BufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(const unsigned int& cnt){
		mElements.push_back({ GL_UNSIGNED_INT, cnt, GL_FALSE });
		mStride += cnt * BufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}
	
	template<>
	void Push<unsigned char>(const unsigned int& cnt){
		mElements.push_back({ GL_UNSIGNED_BYTE, cnt, GL_TRUE });
		mStride += cnt * BufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
	

	void Push(const BufferLayout& layout) {
		for (BufferElement elem : layout.GetElements()) {
			mElements.push_back(elem);
			mStride += elem.cnt * BufferElement::GetSizeOfType(elem.type);
		}
	}

	inline const std::vector<BufferElement>& GetElements() const { return mElements; }
	inline unsigned int GetStride() const { return mStride; }
};