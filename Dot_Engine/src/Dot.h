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
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Renderable/Mesh.h"
#include "Dot/Renderer/Renderable/InstancedMesh.h"
#include "Dot/Renderer/Renderable/AnimatedMesh.h"
#include "Dot/Renderer/RenderSystem.h"
#include "Dot/Renderer/Material.h"

#include "Dot/Utils/Time/Timestep.h"
#include "Dot/Utils/MousePicker.h"
#include "Dot/Utils/Text/Font.h"
#include "Dot/Utils/Random/RandomGenerator.h"

#include "Dot/LevelEditor/Skybox/Skybox.h"
#include "Dot/LevelEditor/Terrain/TerrainEditor.h"
#include "Dot/LevelEditor/Water/WaterTile.h"
#include "Dot/LevelEditor/Terrain/BrushInstance.h"
#include "Dot/LevelEditor/Terrain/Brush.h"


#include "Dot/ECS/ECSManager.h"
#include "Dot/PhysicsEngine/CollisionSystem.h"
#include "Dot/PhysicsEngine/PhysicsSystem.h"
#include "Dot/ParticleEngine/ParticleSystem.h"
#include "Dot/Renderer/AnimationSystem.h"

#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/ParticleEngine/ParticleComponent.h"
#include "Dot/ParticleEngine/ParticleEditor.h"
#include "Dot/Renderer/Renderable/RenderableEditor.h"

#include "Dot/Gui/Gui/GuiApplication.h"
#include "Dot/Gui/Gui/GuiBlock.h"
#include "Dot/Gui/Gui/Widgets/Button.h"
#include "Dot/Gui/Gui/Widgets/ArrowButton.h"
#include "Dot/Gui/Gui/Widgets/CheckBox.h"
#include "Dot/Gui/Gui/Widgets/Slider.h"
#include "Dot/Gui/Gui/Widgets/TextArea.h"
#include "Dot/Gui/Gui/Widgets/Console.h"
#include "Dot/Gui/Gui/Widgets/Panel.h"
#include "Dot/Gui/Gui/Widgets/GuiWindow.h"
#include "Dot/Gui/Gui/Widgets/Dropdown.h"
#include "Dot/Gui/Gui/Layout.h"


#include "Dot/LevelEditor/LevelEditor.h"