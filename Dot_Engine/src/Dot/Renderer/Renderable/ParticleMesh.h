#pragma once
#include "Renderable.h"

namespace Dot {

	class ParticleMesh : public Renderable
	{
	public:
		ParticleMesh();
		ParticleMesh(const Ref<ArrayBuffer> vao);
		void SetRenderCount(int count) { m_Count = count; }

		virtual void Render(const Ref<Shader>& shader, int drawMod)override;
		virtual Ref<ArrayBuffer>& GetVAO() { return m_VAO; }
	private:
		Ref<ArrayBuffer>m_VAO;
		int m_Count;
	};

}