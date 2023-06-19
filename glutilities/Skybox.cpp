#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Skybox.h"
#include "../libs/stb/stb_image.h"
#include <iostream>
#include "../common/GLShader.h"

#include "../glutilities/VertexArray.h"
#include "../glutilities/VertexBuffer.h"
#include "../glutilities/BufferLayout.h"
#include <string>
Skybox::Skybox(const std::string& textureName)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

	std::string ext[] = { "px", "nx", "py", "ny", "pz", "nz" };
	
	int index = textureName.find_last_of('.');

	_ASSERT(index);

	std::string fileName = textureName.substr(0, index);

	std::string extension = textureName.substr(index);

	int width, height, nrChannels;

	for (unsigned int i = 0; i < 6; i++)
	{
		std::string file = fileName + "_" + ext[i] + extension;
		uint8_t* m_TextureData = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);

		if (m_TextureData)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_TextureData);
			stbi_image_free(m_TextureData);
		}
		else
		{
			std::cout << "Failed to load texture" << file << std::endl;
			stbi_image_free(m_TextureData);
		}
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	m_Shader.LoadVertexShader("../shaders/skybox.vs");
	m_Shader.LoadFragmentShader("../shaders/skybox.fs");
	m_Shader.Create();

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
	
	m_VA = new VertexArray();
	m_VB = new VertexBuffer(skyboxVertices, sizeof(skyboxVertices));
	BufferLayout layout;
	layout.Push<float>(3);
	(*m_VA).AddBuffer(*m_VB, layout);
}
Skybox::~Skybox()
{
	glDeleteTextures(1, &m_ID);
	delete m_VA;
	delete m_VB;
}


void Skybox::Render(){
	glDepthMask(GL_FALSE);
	auto program = m_Shader.GetProgram();
	glUseProgram(program);
	m_VA->Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}
