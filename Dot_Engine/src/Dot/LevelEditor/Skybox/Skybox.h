#pragma once
#include "Dot/Renderer/Renderable/Renderable.h"

namespace Dot {
	class Skybox : public Renderable
	{
	public:
		Skybox(float Size);
		~Skybox();

		virtual void Render(const Ref<Shader>& shader, int drawMod) override;
		Ref<ArrayBuffer> vao;
		float size;
	};

}