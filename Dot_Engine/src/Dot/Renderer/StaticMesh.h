#pragma once
#include "Renderable.h"

namespace Dot {

	class Mesh : public Renderable
	{
	public:
		Mesh(const Ref<ArrayBuffer> vao);
		virtual void Render(const Ref<Shader>& shader, int drawMod)override;

		inline Ref<ArrayBuffer>& GetVAO() {return m_VAO;}
	private:
		Ref<ArrayBuffer>m_VAO;
	};

	class StaticMesh : public Renderable
	{
	public:
		StaticMesh(const std::string& filename);
		~StaticMesh();

		virtual void Render(const Ref<Shader>& shader, int drawMod) override;

		inline Ref<ArrayBuffer>& GetVAO() { return m_VAO; }
		inline const glm::vec3& GetMin() const { return m_Min; }
		inline const glm::vec3& GetMax() const { return m_Max; }

		inline const unsigned int GetNumVertices() { return m_NumVertices; }
	private:
		Ref<ArrayBuffer> m_VAO;

		std::vector<StaticVertex> m_Vertices;
		std::vector<Index> m_Indices;
		unsigned int m_NumVertices;

	private:
		glm::vec3 m_Min;
		glm::vec3 m_Max;
	};


}