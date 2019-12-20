#include "stdafx.h"
#include "AssetManager.h"

#include "Dot/Utils/Parser/JsonParser.h"

namespace Dot {
	Scope<AssetManager> AssetManager::s_Instance = std::make_unique<AssetManager>();

	AssetManager::AssetManager()
	{
	}
	void AssetManager::LoadAssets(const std::string& file)
	{
		JsonParser parser;
		Json json;

		std::vector<std::string> keywords;
		keywords.push_back("Textures");
		keywords.push_back("CubeMaps");
		keywords.push_back("Shaders");
		keywords.push_back("AnimatedMeshes");
		keywords.push_back("InstancedMeshes");
		keywords.push_back("Meshes");

		json = parser.ParseJson(file, keywords);

		for (auto& it : json.m_Object)
		{
			if (it.first == "Textures")
			{	
				for (int i = 0; i < it.second.size(); ++i)
				{
					TextureAsset asset;

					asset.name = it.second[i].m_Attribute[0].m_Value.second;
					asset.path = it.second[i].m_Attribute[1].m_Value.second;
					
					if (it.second[i].m_Attribute[2].m_Value.second == "true")
						asset.filters = true;
					else
						asset.filters = false;
					
					if (it.second[i].m_Attribute[3].m_Value.second == "true")
						asset.flipped = true;
					else
						asset.flipped = false;

					LOG_INFO("Loading texture %s",asset.path.c_str());
					m_Texture[asset.name] = Texture2D::Create(asset.path, asset.filters, asset.flipped);
				
				}
			}
			else if (it.first == "Shaders")
			{
				for (int i = 0; i < it.second.size(); ++i)
				{
					ShaderAsset asset;

					asset.name = it.second[i].m_Attribute[0].m_Value.second;
					asset.path = it.second[i].m_Attribute[1].m_Value.second;

					LOG_INFO("Loading shader %s", asset.path.c_str());
					m_Shader[asset.name] = Shader::Create(asset.name, asset.path);
				}

			}
			else if (it.first == "CubeMaps")
			{
				for (int i = 0; i < it.second.size(); ++i)
				{
					CubeMapAsset asset;

					asset.name = it.second[i].m_Attribute[0].m_Value.second;
					for (int j = 1; j < it.second[i].m_Attribute.size(); ++j)
						asset.face.push_back(it.second[i].m_Attribute[j].m_Value.second);

					LOG_INFO("Loading shader %s", asset.face[0].c_str());
					m_CubeMap[asset.name] = CubeMapTexture::Create(asset.face);
				}
			}
			else if (it.first == "AnimatedMeshes")
			{		
				for (int i = 0; i < it.second.size(); ++i)
				{
					AnimatedMeshAsset asset;
					asset.name = it.second[i].m_Attribute[0].m_Value.second;
					asset.path = it.second[i].m_Attribute[1].m_Value.second;

					LOG_INFO("Loading animated mesh %s", asset.path.c_str());
					m_AnimatedMesh[asset.name] = std::make_shared<AnimatedMesh>(asset.path);
				}
			}
			else if (it.first == "Meshes")
			{
				for (int i = 0; i < it.second.size(); ++i)
				{
					MeshAsset asset;
					asset.name = it.second[i].m_Attribute[0].m_Value.second;
					asset.path = it.second[i].m_Attribute[1].m_Value.second;

					LOG_INFO("Loading mesh %s", asset.path.c_str());
					m_Mesh[asset.name] = std::make_shared<Mesh>(asset.path.c_str());
				}
			}
			else if (it.first == "InstancedMeshes")
			{
				for (int i = 0; i < it.second.size(); ++i)
				{
					InstancedMeshAsset asset;
					asset.name = it.second[i].m_Attribute[0].m_Value.second;
					asset.path = it.second[i].m_Attribute[1].m_Value.second;
					asset.capacity = atoi(it.second[i].m_Attribute[2].m_Value.second.c_str());

					std::vector<glm::mat4>transforms;

					LOG_INFO("Loading instanced mesh %s", asset.path.c_str());
					m_InstancedMesh[asset.name] = std::make_shared<InstancedMesh>(asset.path,asset.capacity,transforms);
				}
			}
			
		}
	}

	void AssetManager::PushTexture(const std::string& name, Ref<Texture2D> asset)
	{
		m_Texture[name] = asset;
	}

	void AssetManager::PushCubeMap(const std::string& name, Ref<CubeMapTexture> asset)
	{
		m_CubeMap[name] = asset;
	}

	void AssetManager::PushAnimatedMesh(const std::string& name, Ref<AnimatedMesh> asset)
	{
		m_AnimatedMesh[name] = asset;
	}

	void AssetManager::PushMesh(const std::string& name, Ref<Mesh> asset)
	{
		m_Mesh[name] = asset;
	}

	void AssetManager::PushInstancedMesh(const std::string& name, Ref<InstancedMesh> asset)
	{
		m_InstancedMesh[name] = asset;
	}

	void AssetManager::PushShader(const std::string& name, Ref<Shader> asset)
	{
		m_Shader[name] = asset;
	}
	
	Ref<Texture2D> AssetManager::GetTexture(const std::string& asset)
	{
		if (m_Texture.find(asset) != m_Texture.end())
			return m_Texture[asset];
		else
			D_ASSERT("Texture %s is not loaded!", asset);
	}
	Ref<CubeMapTexture> AssetManager::GetCubeMap(const std::string& asset)
	{
		if (m_CubeMap.find(asset) != m_CubeMap.end())
			return m_CubeMap[asset];
		else
			D_ASSERT("Cubemap %s is not loaded!", asset);
	}
	Ref<AnimatedMesh> AssetManager::GetAnimMesh(const std::string& asset)
	{
		if (m_AnimatedMesh.find(asset) != m_AnimatedMesh.end())
			return m_AnimatedMesh[asset];
		else
			D_ASSERT("Animated mesh %s is not loaded!", asset);
		
	}
	Ref<Mesh> AssetManager::GetMesh(const std::string& asset)
	{
		if (m_Mesh.find(asset) != m_Mesh.end())
			return m_Mesh[asset];
		else
			D_ASSERT("Mesh %s is not loaded!", asset);
	}
	Ref<InstancedMesh> AssetManager::GetInstancedMesh(const std::string& asset)
	{
		if (m_InstancedMesh.find(asset) != m_InstancedMesh.end())
			return m_InstancedMesh[asset];
		else
			D_ASSERT("Instanced mesh %s is not loaded!", asset);
	}
	Ref<Shader> AssetManager::GetShader(const std::string& asset)
	{
		if (m_Shader.find(asset) != m_Shader.end())
			return m_Shader[asset];
		else
			D_ASSERT("Shader %s is not loaded!",asset);
	}

	std::vector<std::string> AssetManager::TokenizeString(const std::string& str, const std::string& delimiter)
	{
		std::vector<std::string> strings;
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = str.find(delimiter, prev)) != std::string::npos) 
		{
			strings.push_back(str.substr(prev, pos - prev));
			prev = pos + 1;
		}

		// To get the last substring (or only, if delimiter is not found)
		strings.push_back(str.substr(prev));
		return strings;
	}
	
}