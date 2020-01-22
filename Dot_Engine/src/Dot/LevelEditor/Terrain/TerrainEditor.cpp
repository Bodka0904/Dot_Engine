#include "stdafx.h"
#include "TerrainEditor.h"
#include "Dot/Utils/Random/RandomGenerator.h"
#include "Dot/Core/Input.h"
#include "Dot/Utils/Image/ImageLoader.h"

#include "Dot/Core/KeyCodes.h"
#include "Dot/Core/MouseButtonCodes.h"
#include "Dot/Renderer/RenderSystem.h"


#include "Dot/Core/AssetManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>



namespace Dot {

	void TerrainEditor::OnCreate(Entity entity)
	{
		
		m_Terrain = std::make_shared<Terrain>(300, 257);
		m_Material = std::make_shared<Material>(AssetManager::Get()->GetShader("TerrainShader"));
		
		Dot::MaterialStack::Get()->RegisterMaterial("Terrain",m_Material);

		m_Material->Set(AssetManager::Get()->GetTexture("TerrainTexture"));
		m_Material->Set(RenderMode::OPAQUE);
		ECSManager::Get()->AddComponent(entity, RenderComponent{
			m_Terrain,
			Dot::MaterialStack::Get()->GetMaterialID("Terrain"),
			D_TRIANGLES
			});
		ECSManager::Get()->AddComponent(entity, Transform{});
		m_Transform = &ECSManager::Get()->GetComponent<Transform>(entity);

		glm::vec2 wrapSizeGen(200, 100);
		//Dot::Wrapper::Create("Terrain Generator", glm::vec2(400, 400), wrapSizeGen, 2);
		//Gui::Get()->EnableWrapper("Terrain Generator");
		//{
		//	Button::Create("Diamond Square", glm::vec2(300, 300), glm::vec2(40, 40));
		//	Button::Create("Height Noise", glm::vec2(300, 300), glm::vec2(40, 40));
		//
		//}Gui::Get()->DisableWrapper();
		//
		//Wrapper::Create("Terrain Brush", glm::vec2(400, 400), glm::vec2(300, 400), 10);
		//Gui::Get()->EnableWrapper("Terrain Brush");
		//{
		//	Slider::Create("Radius", glm::vec2(100, 100), glm::vec2(200, 20), &m_Brush.m_Radius, 0, 40);
		//	Slider::Create("Strength", glm::vec2(100, 100), glm::vec2(200, 20), &m_Brush.m_Strength, -10, 10);
		//	Slider::Create("Smooth Factor", glm::vec2(100, 100), glm::vec2(200, 20), &m_Brush.m_Smooth, 0, 1);
		//	Checkbox::Create("Smooth", glm::vec2(100, 100), glm::vec2(50, 50));
		//}Gui::Get()->DisableWrapper();
		//
		//glm::vec2 wrapSizeTransform(300, 300);
		//glm::vec2 wrapSizePos = glm::vec2(Input::GetWindowSize() - wrapSizeTransform) - glm::vec2(0, wrapSizeGen.y);
		//Wrapper::Create("Terrain Transform", wrapSizePos, wrapSizeTransform, 3);
		//Gui::Get()->EnableWrapper("Terrain Transform");
		//{
		//	TextArea::Create("Pos x", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
		//	TextArea::Create("Pos y", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
		//	TextArea::Create("Pos z", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
		//
		//	TextArea::Create("Rot x", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
		//	TextArea::Create("Rot y", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
		//	TextArea::Create("Rot z", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
		//
		//	TextArea::Create("Scale x", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
		//	TextArea::Create("Scale y", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
		//	TextArea::Create("Scale z", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
		//
		//}Gui::Get()->DisableWrapper();
	}


	void TerrainEditor::OnUpdate()
	{
		//if (Button::GetWrapped("Terrain Generator", "Diamond Square").GetClicked())
		//{
		//	DiamondSquare();
		//	ApplyHeights();
		//	ApplyNormals();
		//}
		////else if (Button::GetWrapped("Terrain Generator", "Height Noise").GetClicked())
		////{
		////	HeightsValueNoise();
		////	ApplyHeights();
		////	ApplyNormals();
		////}
	}

	void TerrainEditor::OnEvent(Event& event)
	{
		//if (event.GetEventType() == EventType::KeyPressed)
		//{
		//	TextArea& m_PosX = TextArea::GetWrapped("Terrain Transform", "Pos x");
		//	TextArea& m_PosY = TextArea::GetWrapped("Terrain Transform", "Pos y");
		//	TextArea& m_PosZ = TextArea::GetWrapped("Terrain Transform", "Pos z");
		//
		//	TextArea& m_RotX = TextArea::GetWrapped("Terrain Transform", "Rot x");
		//	TextArea& m_RotY = TextArea::GetWrapped("Terrain Transform", "Rot y");
		//	TextArea& m_RotZ = TextArea::GetWrapped("Terrain Transform", "Rot z");
		//
		//	TextArea& m_ScaleX = TextArea::GetWrapped("Terrain Transform", "Scale x");
		//	TextArea& m_ScaleY = TextArea::GetWrapped("Terrain Transform", "Scale y");
		//	TextArea& m_ScaleZ = TextArea::GetWrapped("Terrain Transform", "Scale z");
		//
		//	KeyPressedEvent& e = (KeyPressedEvent&)event;
		//
		//	if (m_PosX.GetClicked())
		//	{
		//		m_PosX.TakeInput(e);
		//		m_Transform->pos.x = m_PosX.GetValue<float>();
		//		e.IsHandled();
		//	}
		//	else if (m_PosY.GetClicked())
		//	{
		//		m_PosY.TakeInput(e);
		//		m_Transform->pos.y = m_PosY.GetValue<float>();
		//		e.IsHandled();
		//	}
		//	else if (m_PosZ.GetClicked())
		//	{
		//		m_PosZ.TakeInput(e);
		//		m_Transform->pos.z = m_PosZ.GetValue<float>();
		//		e.IsHandled();
		//	}
		//	else if (m_RotX.GetClicked())
		//	{
		//		m_RotX.TakeInput(e);
		//		m_Transform->rot.x = m_RotX.GetValue<float>();
		//		e.IsHandled();
		//	}
		//	else if (m_RotY.GetClicked())
		//	{
		//		m_RotY.TakeInput(e);
		//		m_Transform->rot.y = m_RotY.GetValue<float>();
		//		e.IsHandled();
		//	}
		//	else if (m_RotZ.GetClicked())
		//	{
		//		m_RotZ.TakeInput(e);
		//		m_Transform->rot.z = m_RotZ.GetValue<float>();
		//		e.IsHandled();
		//	}
		//	else if (m_ScaleX.GetClicked())
		//	{
		//		m_ScaleX.TakeInput(e);
		//		m_Transform->scale.x = m_ScaleX.GetValue<float>();
		//		e.IsHandled();
		//	}
		//	else if (m_ScaleY.GetClicked())
		//	{
		//		m_ScaleY.TakeInput(e);
		//		m_Transform->scale.y = m_ScaleY.GetValue<float>();
		//		e.IsHandled();
		//	}
		//	else if (m_ScaleZ.GetClicked())
		//	{
		//		m_ScaleZ.TakeInput(e);
		//		m_Transform->scale.z = m_ScaleZ.GetValue<float>();
		//		e.IsHandled();
		//	}
		//
		//	//m_Transform->UpdateModel();
		//}
		//if (event.GetEventType() == EventType::MouseButtonPressed)
		//{
		//	if (Input::IsKeyPressed(D_KEY_LEFT_SHIFT))
		//	{
		//		m_Brush.ApplyBrush(this);
		//		//ApplyNormals();
		//	}
		//}
	}

	void TerrainEditor::OnDestroy()
	{
	}


	void TerrainEditor::UpdateBrush(const Camera& camera)
	{
		glm::vec3 pos = m_Brush.m_Position;
		m_Picker.CalculateMouseRay(camera);
		pos = m_Picker.TestIntersectionTerr(camera, m_Terrain);
		m_Brush.SetPosition(pos);
		
		m_Material->Set("u_Radius", m_Brush.m_Radius);
		m_Material->Set("u_BrushPosition", glm::vec2(m_Brush.m_Position.x, m_Brush.m_Position.z));
	}
	
	void TerrainEditor::LoadHeightMap(const std::string& filePath)
	{
		_BMPIMAGE img = ImageLoader::LoadBMP(filePath);// Num vertices
		img.width;

		if (img.width != img.height)
		{
			LOG_WARN("Only heightmaps with same width and height supported");
		}
		if (img.width != m_Terrain->m_NumVertex)
		{
			LOG_WARN("Number of heights in heightmap differs from number of vertices");
		}

		int count = 0;
		int numVertex = 0;
		if (img.width > m_Terrain->m_NumVertex)
		{
			count = m_Terrain->m_NumVertex * m_Terrain->m_NumVertex;
			numVertex = m_Terrain->m_NumVertex;
		}
		else
		{
			count = img.width * img.width;
			numVertex = img.width;
		}
		
		// Temporary vectors 
		std::vector<glm::vec2> texcoords;
		std::vector<glm::vec3> normals;
		std::vector<int > indices;

		texcoords.resize(count);
		normals.resize(count);
		indices.resize(6 * (m_Terrain->m_NumVertex - 1) * (m_Terrain->m_NumVertex - 1));

		m_Terrain->m_Vertices.resize(count);
		m_Terrain->m_Heights.resize(numVertex);

		int vertexPointer = 0;
		for (int i = 0; i < numVertex; ++i)
		{
			m_Terrain->m_Heights[i].resize(numVertex);
			for (int j = 0; j < numVertex; ++j)
			{
				m_Terrain->m_Heights[i][j] = (float)img.pixels[i][j].r / 255.0f * m_Height;
				m_Terrain->m_Vertices[vertexPointer] = glm::vec4((float)i / ((float)numVertex - 1) * m_Terrain->m_Size, m_Terrain->m_Heights[i][j], (float)j / ((float)m_Terrain->m_NumVertex - 1) * m_Terrain->m_Size,0);
				normals[vertexPointer] = glm::vec3(0, 1, 0);
				texcoords[vertexPointer] = glm::vec2((float)i / ((float)numVertex - 1), (float)j / ((float)numVertex - 1));

				vertexPointer++;
			}
		}
	}

	void TerrainEditor::DiamondSquare()
	{
		diamondSquare(m_Terrain->m_NumVertex);
	}

	void TerrainEditor::HeightsValueNoise()
	{
		int vertexPointer = 0;
		for (int i = 0; i < m_Terrain->m_NumVertex; i++)
		{
			for (int j = 0; j < m_Terrain->m_NumVertex; j++)
			{
				float height = generateHeight(i, j) * m_Height;
				m_Terrain->m_Heights[j][i] = height;
			}
		}
	}

	void TerrainEditor::ApplyNormals()
	{
		int vertexPointer = 0;
		for (int i = 0; i < m_Terrain->m_NumVertex; i++)
		{
			for (int j = 0; j < m_Terrain->m_NumVertex; j++)
			{
				m_Terrain->m_Normals[vertexPointer] = glm::vec4(generateNormal(j, i),1);
				vertexPointer++;
			}
		}
		m_Terrain->m_VAO->GetStorageBuffer(1)->Update(&m_Terrain->m_Normals[0], m_Terrain->m_Normals.size() * sizeof(glm::vec4), 0);
	}

	void TerrainEditor::ApplyHeights()
	{
		int vertexPointer = 0;
		for (int i = 0; i < m_Terrain->m_NumVertex; i++)
		{
			for (int j = 0; j < m_Terrain->m_NumVertex; j++)
			{
				m_Terrain->m_Vertices[vertexPointer].y = m_Terrain->m_Heights[j][i];
				vertexPointer++;
			}
		}
		m_Terrain->m_VAO->GetStorageBuffer(0)->Update(&m_Terrain->m_Vertices[0], m_Terrain->m_Vertices.size() * sizeof(glm::vec4), 0);
	}


	float TerrainEditor::getNoise(int x, int z)
	{
		std::mt19937 rng(x + z + m_Seed);
		std::uniform_real_distribution<> dist(-1, 1);

		return dist(rng);
	}

	float TerrainEditor::getSmoothNoise(int x, int z)
	{
		float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1)
			+ getNoise(x + 1, z + 1)) / 16.0f;
		float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1)
			+ getNoise(x, z + 1)) / 8.0f;
		float center = getNoise(x, z) / 4.0f;

		return corners + sides + center;
	}

	float TerrainEditor::getInterpolatedNoise(float x, float z)
	{
		int intX = (int)x;
		int intZ = (int)z;
		float fracX = x - intX;
		float fracZ = z - intZ;


		float v1 = getSmoothNoise(intX, intZ);
		float v2 = getSmoothNoise(intX + 1, intZ);
		float v3 = getSmoothNoise(intX, intZ + 1);
		float v4 = getSmoothNoise(intX + 1, intZ + 1);
		float i1 = interpolateCosine(v1, v2, fracX);
		float i2 = interpolateCosine(v3, v4, fracX);


		return interpolateCosine(i1, i2, fracZ);
	}

	float TerrainEditor::interpolateCosine(float a, float b, float blend)
	{
		double theta = blend * M_PI;
		float f = (float)(1.0f - cos(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}

	float TerrainEditor::generateHeight(int x, int z)
	{
		float total = 0;
		float d = (float)pow(2, m_Octaves - 1);
		for (int i = 0; i < m_Octaves; i++) {
			float freq = (float)(pow(2, i) / d);
			float amp = (float)pow(m_Roughness, i) *m_Amplitude;
			total += getInterpolatedNoise((x)* freq, (z * freq) * amp);
		}
		return total;
	}

	float TerrainEditor::getHeight(int x, int z) const
	{
		if (x < 0 || x >= m_Terrain->m_Heights.size() || z < 0 || z >= m_Terrain->m_Heights[0].size())
		{
			return 0;
		}
		else
		{
			return m_Terrain->m_Heights[x][z];
		}
	}

	glm::vec3 TerrainEditor::generateNormal(int x, int z) const
	{
		float heightL = getHeight(x - 1, z);
		float heightR = getHeight(x + 1, z);
		float heightD = getHeight(x, z - 1);
		float heightU = getHeight(x, z + 1);

		glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f, heightD - heightU);

		return glm::normalize(normal);
	}

	void TerrainEditor::diamondSquare(int size)
	{
		int half = size / 2;
		if (half < 1)
			return;

		for (int z = half; z < m_Terrain->m_NumVertex; z += size)
		{
			for (int x = half; x < m_Terrain->m_NumVertex; x += size)
			{
				squareStep(x % m_Terrain->m_NumVertex, z % m_Terrain->m_NumVertex, half);
			}
		}

		int col = 0;
		for (int x = 0; x < m_Terrain->m_NumVertex; x += half)
		{
			col++;
			if (col % 2 == 1)
			{
				for (int z = half; z < m_Terrain->m_NumVertex; z += size)
				{
					diamondStep(x % m_Terrain->m_NumVertex, z % m_Terrain->m_NumVertex, half);
				}
			}
			else
			{
				for (int z = 0; z < m_Terrain->m_NumVertex; z += size)
				{
					diamondStep(x % m_Terrain->m_NumVertex, z % m_Terrain->m_NumVertex, half);
				}
			}
		}
		diamondSquare(size / 2);
	}
	void TerrainEditor::squareStep(int x, int z, int reach)
	{
		int count = 0;
		float avg = 0.0f;
		if (x - reach >= 0 && z - reach >= 0)
		{
			avg += m_Terrain->m_Heights[x - reach][z - reach];
			count++;
		}
		if (x - reach >= 0 && z + reach < m_Terrain->m_NumVertex)
		{
			avg += m_Terrain->m_Heights[x - reach][z + reach];
			count++;
		}
		if (x + reach < m_Terrain->m_NumVertex && z - reach >= 0)
		{
			avg += m_Terrain->m_Heights[x + reach][z - reach];
			count++;
		}
		if (x + reach < m_Terrain->m_NumVertex && z + reach < m_Terrain->m_NumVertex)
		{
			avg += m_Terrain->m_Heights[x + reach][z + reach];
			count++;
		}
		avg += RandomGenerator::Random(double(reach) / 2, reach) * m_Height;
		avg /= count;
		m_Terrain->m_Heights[x][z] = round(avg);
	}

	void TerrainEditor::diamondStep(int x, int z, int reach)
	{	
		int count = 0;
		float avg = 0.0f;
		if (x - reach >= 0)
		{
			avg += m_Terrain->m_Heights[x - reach][z];
			count++;
		}
		if (x + reach < m_Terrain->m_NumVertex)
		{
			avg += m_Terrain->m_Heights[x + reach][z];
			count++;
		}
		if (z - reach >= 0)
		{
			avg += m_Terrain->m_Heights[x][z - reach];
			count++;
		}
		if (z + reach < m_Terrain->m_NumVertex)
		{
			avg += m_Terrain->m_Heights[x][z + reach];
			count++;
		}
		avg += RandomGenerator::Random(double(reach) / 2, reach) * m_Height;
		avg /= count;
		m_Terrain->m_Heights[x][z] = (int)avg;
	}
	TerrainEditor::TerrainBrush::TerrainBrush()
		: m_Radius(1.0f),m_Strength(1.0f),m_Smooth(1.0f),m_Position(glm::vec3(1))
	{
		AssetManager::Get()->LoadShader("res/Shaders/Terrain/TerrainBrushCompute.glsl", "ComputeBrush");
		m_ComputeBrush = AssetManager::Get()->GetShader("ComputeBrush");
	}
	Dot::TerrainEditor::TerrainBrush::~TerrainBrush()
	{
		AssetManager::Get()->UnLoadShader("ComputeBrush");
	}
	void TerrainEditor::TerrainBrush::ApplyBrush(TerrainEditor* editor)
	{	
		float edgeSize = editor->m_Terrain->m_Size / editor->m_Terrain->m_NumVertex;
		m_ComputeBrush->Bind();
		m_ComputeBrush->UploadUniformFloat("u_Radius",m_Radius);
		m_ComputeBrush->UploadUniformFloat("u_Strength", m_Strength);
		m_ComputeBrush->UploadUniformFloat("u_SmoothFactor", m_Smooth);
		m_ComputeBrush->UploadUniformFloat("u_EdgeSize", edgeSize);
		m_ComputeBrush->UploadUniformInt("u_Width", editor->m_Terrain->m_NumVertex);
		//if (Checkbox::GetWrapped("Terrain Brush", "Smooth").GetClicked())
		//{
		//	m_ComputeBrush->UploadUniformInt("u_Smooth", true);
		//}
		//else
		//{
		//	m_ComputeBrush->UploadUniformInt("u_Smooth", false);
		//}
		
		m_ComputeBrush->UploadUniformFloat2("u_BrushPosition", glm::vec2(m_Position.x, m_Position.z));
		editor->m_Terrain->m_VAO->GetStorageBuffer(0)->BindRange(0, editor->m_Terrain->m_Vertices.size() * sizeof(glm::vec4),0);
		editor->m_Terrain->m_VAO->GetStorageBuffer(1)->BindRange(0, editor->m_Terrain->m_Normals.size() * sizeof(glm::vec4),1);
		m_ComputeBrush->Compute(32, 32, 1);
	}
	float TerrainEditor::TerrainBrush::smoothPoint(int x, int z, const Ref<Terrain>& terrain)
	{
		float left = terrain->m_Heights[x - 1][z];
		float right = terrain->m_Heights[x + 1][z];
		float bottom = terrain->m_Heights[x][z - 1];
		float top = terrain->m_Heights[x][z + 1];

		return (left + right + bottom + top) / 4;
	}
}