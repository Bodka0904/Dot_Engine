#pragma once


#include "Dot/Core/Application.h"
#include "Dot/Core/Layer.h"
#include "Dot/Debug/Log.h"

#include "Dot/Core/Input.h"
#include "Dot/Core/KeyCodes.h"
#include "Dot/Core/MouseButtonCodes.h"


#include "Dot/Renderer/Buffers/FrameBuffer.h"
#include "Dot/Renderer/Renderer.h"
#include "Dot/Renderer/Camera/Camera.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shaders/Shader.h"
#include "Dot/Renderer/Mesh.h"
#include "Dot/Renderer/Material/Light.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/AnimatedMesh.h"

#include "Dot/Utils/Time/Timestep.h"
#include "Dot/Utils/MousePicker.h"
#include "Dot/Utils/Text/Font.h"
#include "Dot/Utils/Skybox.h"
#include "Dot/utils/Transform.h"
#include "Dot/Utils/Particle/ParticleManager.h"
#include "Dot/Utils/Terrain/TerrainEditor.h"
#include "Dot/Utils/Terrain/Water.h"


// --- Widgets --------------
#include "Dot/Gui/GuiSystem/Slider.h"
#include "Dot/Gui/GuiSystem/Checkbox.h"
#include "Dot/Gui/GuiSystem/ArrowButton.h"
#include "Dot/Gui/GuiSystem/Button.h"
#include "Dot/Gui/GuiSystem/Gui.h"

// --- Entry Point -----------------
#include "Dot/Core/EntryPoint.h"
// ---------------------------------