#pragma once

#include <cstdint>
#include <string>

class GLShader
{
	private:
		std::string mName;
		uint32_t m_Program;
		uint32_t m_VertexShader;
		uint32_t m_GeometryShader;
		uint32_t m_FragmentShader;

		bool CompileShader(uint32_t type);
	public:
		GLShader(std::string name = "") : m_Program(0), m_VertexShader(0),m_GeometryShader(0), m_FragmentShader(0), mName(name) {}
		~GLShader() {}

		inline uint32_t GetProgram() { return m_Program; }

		bool LoadVertexShader(const char* fileName);
		bool LoadGeometryShader(const char* fileName);
		bool LoadFragmentShader(const char* fileName);
		bool Create();
		void Destroy();
		std::string GetName() { return mName; }
};