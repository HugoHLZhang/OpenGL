#pragma once

#include <cstdint>
#include <string>

class GLShader
{
	private:
		std::string m_Name;
		uint32_t m_Program;
		uint32_t m_VertexShader;
		uint32_t m_GeometryShader;
		uint32_t m_FragmentShader;

		bool CompileShader(uint32_t type);
	public:
		GLShader(std::string name = "") : m_Program(0), m_VertexShader(0),m_GeometryShader(0), m_FragmentShader(0), m_Name(name) {}
		~GLShader() {}

		inline uint32_t GetProgram() { return m_Program; }

		bool LoadVertexShader(const char* filename);
		bool LoadGeometryShader(const char* filename);
		bool LoadFragmentShader(const char* filename);
		bool Create();
		void Destroy();
		std::string GetName() { return m_Name; }
};