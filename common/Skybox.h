#pragma once
#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "../common/Texture.h"
#include <vector>
#include "../common/GLShader.h"

#include "../common/VertexArray.h"
#include "../common/VertexBuffer.h"
#include <string>
class Skybox {
	
private:
	unsigned int mID;
	VertexArray* mVA;
	VertexBuffer* mVB;
	GLShader mShader;
	
public:
	Skybox(const std::string& texName);
	~Skybox();
	GLShader GetShader() const { return mShader; }
	void Render();
};

