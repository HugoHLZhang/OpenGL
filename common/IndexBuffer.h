#pragma once
class IndexBuffer {
	
private:
	unsigned int mID;
	unsigned int mCount;
public:
	IndexBuffer(const unsigned int* data, const unsigned int& size);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return mCount; }
};