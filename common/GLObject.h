#pragma once
#include "../common/ObjData.h"
#include "../common/GLShader.h"
#include "../common/VertexArray.h"
#include "../common/VertexBuffer.h"
#include "../common/BufferLayout.h"
#include "../common/Texture.h"
#include "../common/IndexBuffer.h"
#include <vector>
#include "../common/Matrix.h"
#include "../common/Vertex.h"
#include <string>



class GLObject {
	
	private:
		std::string mName;
		std::vector<std::string> mObjectName;
		Vector3 mPos;
		Vector3 mRot;
		Vector3 mScale;
		std::vector<VertexArray*> mVAs;
		std::vector<VertexBuffer*> mVBs;
		GLShader mGLShader;
		std::vector<Texture*> mText;
		std::vector<Material> mMat;
		std::vector<size_t> mCounts;

	public:
		GLObject(const GLShader& glshader, const Vector3& position = Vector3(0, 0, 0), const Vector3& rotation = Vector3(0, 0, 0), const Vector3& scale = Vector3(1, 1, 1));

		~GLObject();
		
		Vector3& GetPosition() { return mPos; }
		Vector3& GetRotation() { return mRot; }
		Vector3& GetScale() { return mScale; }
	
		Vector3* GetpPosition() { return &mPos; }
		Vector3* GetpRotation() { return &mRot; }
		Vector3* GetpScale() { return &mScale; }
	
		void AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout);
		void AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout, const Material& materials);
		void AddShape(const std::vector<Vertex>& vertices, const BufferLayout& layout, const Material& materials, const std::string& texName);

		void SetName(const std::string& name) { mName = name; }
		std::string GetName() const { return mName; }
		std::string GetShaderName() { return mGLShader.GetName(); }
		void SetShader(const GLShader& glshader) { mGLShader = glshader; }

		void SetObjectName(const std::string& name) { mObjectName.push_back(name); }

		Matrix4x4 GetTransform() const { return Matrix4x4::GetTransform(mPos, mRot, mScale); }

		void Render();
};

