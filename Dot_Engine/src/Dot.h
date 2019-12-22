#pragma once


#include "Dot/Core/Application.h"
#include "Dot/Core/AssetManager.h"
#include "Dot/Core/Layer.h"
#include "Dot/Debug/Log.h"

#include "Dot/Core/Input.h"
#include "Dot/Core/KeyCodes.h"
#include "Dot/Core/MouseButtonCodes.h"
#include "Dot/Core/AssetManager.h"

#include "Dot/Renderer/Buffers/FrameBuffer.h"
#include "Dot/Renderer/Renderer.h"
#include "Dot/Renderer/Camera/Camera.h"
#include "Dot/Renderer/Camera/CameraController.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shaders/Shader.h"
#include "Dot/Renderer/Mesh.h"
#include "Dot/Renderer/Material/Light.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/AnimatedMesh.h"

#include "Dot/Utils/Time/Timestep.h"
#include "Dot/Utils/MousePicker.h"
#include "Dot/Utils/Text/Font.h"
#include "Dot/PhysicsEngine/Components/Transform.h"
#include "Dot/Utils/Random/RandomGenerator.h"

#include "Dot/LevelEditor/Skybox/Skybox.h"
#include "Dot/LevelEditor/Particle/ParticleManager.h"
#include "Dot/LevelEditor/Terrain/TerrainEditor.h"
#include "Dot/LevelEditor/Water/Water.h"
#include "Dot/LevelEditor/Terrain/BrushInstance.h"
#include "Dot/LevelEditor/Terrain/Brush.h"

// --- Widgets --------------
#include "Dot/Gui/GuiSystem/Slider.h"
#include "Dot/Gui/GuiSystem/Checkbox.h"
#include "Dot/Gui/GuiSystem/ArrowButton.h"
#include "Dot/Gui/GuiSystem/Button.h"
#include "Dot/Gui/GuiSystem/Gui.h"


#include "Dot/ECS/ECSManager.h"