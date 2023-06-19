#pragma once
#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "../glutilities/Texture.h"
#include <vector>
#include "../common/GLShader.h"

#include "../glutilities/VertexArray.h"
#include "../glutilities/VertexBuffer.h"
#include <string>
class Skybox {
	
private:
	unsigned int m_ID;
	GLShader m_Shader;
	VertexArray* m_VA;
	VertexBuffer* m_VB;
	
public:
	Skybox(const std::string& textureName);
	~Skybox();

	GLShader getShader() const { return m_Shader; }
	void Render();
};

