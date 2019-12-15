#include "stdafx.h"
#include "RenderCommand.h"

namespace Dot {
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}