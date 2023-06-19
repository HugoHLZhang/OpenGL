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

Texture::Texture(const std::string& path, const bool& gammaCorrection) : m_ID(0), m_FilePath(path), m_Slot(0), m_TextureData(nullptr), m_GammaCorrection(gammaCorrection)
{
	int nrComponents;
	uint8_t* m_TextureData = stbi_load(path.c_str(), &m_Width, &m_Height, &nrComponents, STBI_rgb_alpha);
	if (m_TextureData != nullptr) {
		
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(gammaCorrection)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureData);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureData);

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(m_TextureData);
		m_TextureData = nullptr;
		m_Slot = Texture::m_TextureCount++;
	}
	else {
		std::cout << "Failed to load texture: " << path << std::endl;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
	if(m_TextureData)
		stbi_image_free(m_TextureData);
	m_TextureData=nullptr;
	
}

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_Slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

