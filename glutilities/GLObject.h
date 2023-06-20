#pragma once
#include "../common/Struct.h"
#include "../common/GLShader.h"
#include "../glutilities/VertexArray.h"
#include "../glutilities/VertexBuffer.h"
#include "../glutilities/BufferLayout.h"
#include "../glutilities/Texture.h"
#include "../glutilities/IndexBuffer.h"
#include <vector>
#include "../common/Matrix.h"
#include "../common/Vector.h"
#include <string>



class GLObject {
	
	private:
		std::string m_Name;
		std::vector<VertexArray*> m_VAs;
		std::vector<VertexBuffer*> m_VBs;
		GLShader m_GLShader;
		std::vector<Texture*> m_Textures;
		std::vector<Material> m_Materials;
		std::vector<size_t> m_Counts;
		std::vector<std::string> m_ObjectName;
		Vector3 m_Position;
		Vector3 m_Rotation;
		Vector3 m_Scale;

	public:
		GLObject(const GLShader& glshader, const Vector3& position = Vector3(0, 0, 0), const Vector3& rotation = Vector3(0, 0, 0), const Vector3& scale = Vector3(1, 1, 1));

		~GLObject();
		void AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout);
		void AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout, const Material& materials);
		void AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout, const Material& materials, const std::string& textureName);
	
		Vector3& GetPosition() { return m_Position; }
		Vector3& GetRotation() { return m_Rotation; }
		Vector3& GetScale() { return m_Scale; }
	
		Vector3* GetpPosition() { return &m_Position; }
		Vector3* GetpRotation() { return &m_Rotation; }
		Vector3* GetpScale() { return &m_Scale; }
	
		void SetName(const std::string& name) { m_Name = name; }
		std::string GetName() const { return m_Name; }
		std::string GetShaderName() { return m_GLShader.GetName(); }
		void SetShader(const GLShader& glshader) { m_GLShader = glshader; }

		void SetObjectName(const std::string& name) { m_ObjectName.push_back(name); }

		Matrix4x4 GetTransform() const { return Matrix4x4::GetTransform(m_Position, m_Rotation, m_Scale); }

		void Render();
};

