#pragma once
#include "Renderable.h"

namespace Dot {

	class Mesh : public Renderable
 	{
	public:
		Mesh(const std::string& filename);
		~Mesh();
		virtual void Render(const Ref<Shader>& shader, int drawMod) override;

		Ref<ArrayBuffer> vao;
		std::vector<StaticVertex> vertices;
		std::vector<Index> indices;
		unsigned int numVertices;
	};
}