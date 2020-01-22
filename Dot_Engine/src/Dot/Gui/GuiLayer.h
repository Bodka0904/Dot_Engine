#pragma once
#include "Dot/Core/Layer.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Buffers/FrameBuffer.h"
#include "Dot/Renderer/RenderSystem.h"

#include "Dot/Renderer/Renderer2D.h"
#include "Dot/Utils/Text/Text.h"
#include "Gui/GuiBlock.h"

namespace Dot {

	class GuiLayer : public Layer
	{
	public:
		GuiLayer();
		~GuiLayer();
		
		virtual void OnAttach() override;
		virtual void OnDetach()override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event)override;

	};
}
