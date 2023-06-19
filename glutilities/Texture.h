#pragma once
#include <string>

class Texture {
	
private:
	unsigned int m_ID;
	std::string m_FilePath;
	int m_Width, m_Height;
	uint8_t* m_TextureData;
	inline static unsigned int m_TextureCount = 1;
	bool m_GammaCorrection;
	unsigned int m_Slot;
public:
	Texture(const std::string& path, const bool& gammaCorrection = true );
	~Texture();

	void Bind() const;
	void Unbind() const;
	unsigned int GetSlot() const{ return m_Slot; }
	bool GetGammaCorrection() const { return m_GammaCorrection; }
};

