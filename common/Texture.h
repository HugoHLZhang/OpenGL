#pragma once
#include <string>

class Texture {
	
private:
	unsigned int mID;
	std::string mFilePath;
	int mWidth, mHeight;
	uint8_t* mTextureData;
	inline static unsigned int mTextureCount = 1;
	bool mGammaCorr;
	unsigned int mSlot;
public:
	Texture(const std::string& path, const bool& gammaCorr = true );
	~Texture();

	void Bind() const;
	void Unbind() const;
	unsigned int GetSlot() const{ return mSlot; }
	bool GetGammaCorrection() const { return mGammaCorr; }
};

