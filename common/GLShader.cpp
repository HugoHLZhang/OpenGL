
//#include "stdafx.h"
#include "GLShader.h"
#define GLEW_STATIC 1
#include "GL/glew.h"

#include <fstream>
#include <iostream>

bool ValidateShader(GLuint shader)
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[1 + infoLen];

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			std::cout << "Error compiling shader:" << infoLog << std::endl;

			delete[] infoLog;
		}
		glDeleteShader(shader);

		return false;
	}

	return true;
}

bool GLShader::LoadVertexShader(const char* fileName)
{
	std::ifstream fin(fileName, std::ios::in | std::ios::binary);
	fin.seekg(0, std::ios::end);
	uint32_t length = (uint32_t)fin.tellg();
	fin.seekg(0, std::ios::beg);
	char* buffer = nullptr;
	buffer = new char[length + 1];
	buffer[length] = '\0';
	fin.read(buffer, length);
	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexShader, 1, &buffer, nullptr);
	glCompileShader(m_VertexShader);
	delete[] buffer;
	fin.close();

	return ValidateShader(m_VertexShader);
}

bool GLShader::LoadGeometryShader(const char* fileName)
{

	std::ifstream fin(fileName, std::ios::in | std::ios::binary);
	fin.seekg(0, std::ios::end);
	uint32_t length = (uint32_t)fin.tellg();
	fin.seekg(0, std::ios::beg);
	char* buffer = nullptr;
	buffer = new char[length + 1];
	buffer[length] = '\0';
	fin.read(buffer, length);


	m_GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(m_GeometryShader, 1, &buffer, nullptr);
	glCompileShader(m_GeometryShader);

	delete[] buffer;
	fin.close();	


	return ValidateShader(m_GeometryShader);
}

bool GLShader::LoadFragmentShader(const char* fileName)
{
	std::ifstream fin(fileName, std::ios::in | std::ios::binary);
	fin.seekg(0, std::ios::end);
	uint32_t length = (uint32_t)fin.tellg();
	fin.seekg(0, std::ios::beg);
	char* buffer = nullptr;
	buffer = new char[length + 1];
	buffer[length] = '\0';
	fin.read(buffer, length);

	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentShader, 1, &buffer, nullptr);
	glCompileShader(m_FragmentShader);
	delete[] buffer;
	fin.close();
	return ValidateShader(m_FragmentShader);
}

bool GLShader::Create()
{
	m_Program = glCreateProgram();
	glAttachShader(m_Program, m_VertexShader);
	if (m_GeometryShader)
		glAttachShader(m_Program, m_GeometryShader);
	glAttachShader(m_Program, m_FragmentShader);
	glLinkProgram(m_Program);

	int32_t linked = 0;
	int32_t infoLen = 0;
	glGetProgramiv(m_Program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen + 1];

			glGetProgramInfoLog(m_Program, infoLen, NULL, infoLog);
			std::cout << "Erreur de lien du programme: " << infoLog << std::endl;

			delete(infoLog);
		}

		glDeleteProgram(m_Program);

		return false;
	}

	return true;
}

void GLShader::Destroy()
{
	glDetachShader(m_Program, m_VertexShader);
	glDetachShader(m_Program, m_FragmentShader);
	glDetachShader(m_Program, m_GeometryShader);
	glDeleteShader(m_GeometryShader);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
	glDeleteProgram(m_Program);
}