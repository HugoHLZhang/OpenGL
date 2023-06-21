#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Texture.h"
#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"
#endif
#include <iostream>
#include <string>

Texture::Texture(const std::string& path, const bool& gammaCorr) : mID(0), mFilePath(path), mSlot(0), mTextureData(nullptr), mGammaCorr(gammaCorr)
{
	int nrComponents;
	uint8_t* mTextureData = stbi_load(path.c_str(), &mWidth, &mHeight, &nrComponents, STBI_rgb_alpha);
	if (mTextureData != nullptr) {
		
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_2D, mID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, (gammaCorr) ? GL_SRGB8 : GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTextureData);

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(mTextureData);
		mTextureData = nullptr;
		mSlot = Texture::mTextureCount++;
	}
	else {
		std::cout << "Failed to load texture : " << path << std::endl;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &mID);
	if(mTextureData)
		stbi_image_free(mTextureData);
	mTextureData=nullptr;
	
}

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + mSlot);
	glBindTexture(GL_TEXTURE_2D, mID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

