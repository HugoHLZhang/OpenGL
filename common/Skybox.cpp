#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Skybox.h"
#include "../libs/stb/stb_image.h"
#include <iostream>
#include "../common/GLShader.h"

#include "../common/VertexArray.h"
#include "../common/VertexBuffer.h"
#include "../common/BufferLayout.h"
#include <string>

Skybox::Skybox(const std::string& texName)
{
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mID);

	std::string ext[] = { "px", "nx", "py", "ny", "pz", "nz" };
	int index = texName.find_last_of('.');
	_ASSERT(index);
	std::string fileName = texName.substr(0, index);
	std::string extension = texName.substr(index);
	int width, height, nrChannels;

	for (unsigned int i = 0; i < 6; i++)
	{
		std::string file = fileName + "_" + ext[i] + extension;
		uint8_t* mTextureData = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);

		if (mTextureData)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, mTextureData);
			stbi_image_free(mTextureData);
		}
		else
		{
			std::cout << "Failed to load texture" << file << std::endl;
			stbi_image_free(mTextureData);
		}
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	mShader.LoadVertexShader("../shaders/skybox.vs");
	mShader.LoadFragmentShader("../shaders/skybox.fs");
	mShader.Create();

	const float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	
	mVA = new VertexArray();
	mVB = new VertexBuffer(skyboxVertices, sizeof(skyboxVertices));
	BufferLayout layout;
	layout.Push<float>(3);
	(*mVA).AddBuffer(*mVB, layout);
}

Skybox::~Skybox()
{
	glDeleteTextures(1, &mID);
	delete mVA;
	delete mVB;
}

void Skybox::Render(){
	glDepthMask(GL_FALSE);

	auto program = mShader.GetProgram();
	glUseProgram(program);

	mVA->Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mID);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}
