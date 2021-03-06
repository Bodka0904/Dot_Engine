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
		m_Texture["Default"].name = "Default";
		m_Texture["Default"].asset = Texture2D::Create(1, 1);
		uint32_t defaultTextureData = 0xffffffff;
		m_Texture["Default"].asset->SetData(&defaultTextureData, sizeof(defaultTextureData));


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

					LOG_INFO("Loading texture %s", asset.path.c_str());
					if (m_Texture.find(asset.name) != m_Texture.end())
					{
						LOG_WARN("Asset with the same name, asset %s overriden", asset.name);
					}
					m_Texture[asset.name].asset = Texture2D::Create(asset.path, asset.filters, asset.flipped);
					m_Texture[asset.name].path = asset.path;
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
					if (m_Shader.find(asset.name) != m_Shader.end())
					{
						LOG_WARN("Asset with the same name, asset %s overriden", asset.name);
					}
					m_Shader[asset.name].asset = Shader::Create(asset.name, asset.path);
					m_Shader[asset.name].path = asset.path;
				}

			}
			else if (it.first == "CubeMaps")
			{
				for (int i = 0; i < it.second.size(); ++i)
				{
					CubeMapAsset asset;

					asset.name = it.second[i].m_Attribute[0].m_Value.second;
					for (int j = 1; j < it.second[i].m_Attribute.size(); ++j)
					{
						asset.face.push_back(it.second[i].m_Attribute[j].m_Value.second);
					}
					LOG_INFO("Loading cubemap %s", asset.face[0].c_str());
					if (m_CubeMap.find(asset.name) != m_CubeMap.end())
					{
						LOG_WARN("Asset with the same name, asset %s overriden", asset.name);
					}
					m_CubeMap[asset.name].asset = CubeMapTexture::Create(asset.face);
					m_CubeMap[asset.name].face = asset.face;
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
					if (m_AnimatedMesh.find(asset.name) != m_AnimatedMesh.end())
					{
						LOG_WARN("Asset with the same name, asset %s overriden", asset.name);
					}

					m_AnimatedMesh[asset.name].asset = std::make_shared<SkinnedMesh>(asset.path);
					m_AnimatedMesh[asset.name].path = asset.path;
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
					if (m_Mesh.find(asset.name) != m_Mesh.end())
					{
						LOG_WARN("Asset with the same name, asset %s overriden", asset.name);
					}
					m_Mesh[asset.name].asset = std::make_shared<Mesh>(asset.path.c_str());
					m_Mesh[asset.name].path = asset.path;
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


					LOG_INFO("Loading instanced mesh %s", asset.path.c_str());
					if (m_InstancedMesh.find(asset.name) != m_InstancedMesh.end())
					{
						LOG_WARN("Asset with the same name, asset %s overriden", asset.name);
					}
					m_InstancedMesh[asset.name].asset = std::make_shared<InstancedMesh>(asset.path, asset.capacity);
					m_InstancedMesh[asset.name].path = asset.path;
				}
			}

		}
	}


	void AssetManager::LoadTexture(const std::string& path, const std::string& name)
	{
		m_Texture[name].path = path;
		m_Texture[name].asset = Texture2D::Create(path, m_Texture[name].filters, m_Texture[name].flipped);
	}

	void AssetManager::LoadShader(const std::string& path, const std::string& name)
	{
		LOG_INFO("Loading shader %s", path.c_str());
		m_Shader[name].path = path;
		m_Shader[name].asset = Shader::Create(name, path);
	}

	void AssetManager::LoadAnimatedMesh(const std::string& path, const std::string& name)
	{
		m_AnimatedMesh[name].path = path;
		m_AnimatedMesh[name].asset = std::make_shared<SkinnedMesh>(path);
	}

	void AssetManager::LoadInstancedMesh(const std::string& path, const std::string& name, int capacity)
	{
		m_InstancedMesh[name].capacity = capacity;
		m_InstancedMesh[name].path = path;
		m_InstancedMesh[name].asset = std::make_shared<InstancedMesh>(path, capacity);
	}

	void AssetManager::LoadStaticMesh(const std::string& path, const std::string& name)
	{
		m_Mesh[name].path = path;
		m_Mesh[name].asset = std::make_shared<Mesh>(path);
	}

	void AssetManager::LoadCubeMap(const std::vector<std::string>& face, const std::string& name)
	{
		LOG_INFO("Loading cubemap %s", face[0].c_str());
		m_CubeMap[name].face = face;
		m_CubeMap[name].asset = CubeMapTexture::Create(m_CubeMap[name].face);
	}

	void AssetManager::UnLoadTexture(const std::string& name)
	{
		m_Texture[name].asset.~shared_ptr();
	}

	void AssetManager::UnLoadShader(const std::string& name)
	{
		m_Shader[name].asset.~shared_ptr();
	}

	void AssetManager::UnLoadAnimatedMesh(const std::string& name)
	{
		m_AnimatedMesh[name].asset.~shared_ptr();
	}

	void AssetManager::UnLoadInstancedMesh(const std::string& name)
	{
		m_InstancedMesh[name].asset.~shared_ptr();
	}

	void AssetManager::UnLoadMesh(const std::string& name)
	{
		m_Mesh[name].asset.~shared_ptr();
	}

	void AssetManager::UnLoadCubeMap(const std::string& name)
	{
		m_CubeMap[name].asset.~shared_ptr();
	}



	Ref<Texture2D> AssetManager::GetTexture(const std::string& asset)
	{
		if (m_Texture.find(asset) != m_Texture.end())
			return m_Texture[asset].asset;
		else
		{
			LOG_ERR("Texture %s is not loaded!", asset.c_str());
			return m_Texture["Default"].asset;
		}
	}
	Ref<CubeMapTexture> AssetManager::GetCubeMap(const std::string& asset)
	{
		if (m_CubeMap.find(asset) != m_CubeMap.end())
			return m_CubeMap[asset].asset;
		else
			LOG_ERR("Cubemap %s is not loaded!", asset.c_str());


	}
	Ref<SkinnedMesh> AssetManager::GetAnimMesh(const std::string& asset)
	{
		if (m_AnimatedMesh.find(asset) != m_AnimatedMesh.end())
			return m_AnimatedMesh[asset].asset;
		else
			LOG_ERR("Animated mesh %s is not loaded!", asset.c_str());

	}
	Ref<Mesh> AssetManager::GetMesh(const std::string& asset)
	{
		if (m_Mesh.find(asset) != m_Mesh.end())
			return m_Mesh[asset].asset;
		else
			LOG_ERR("Mesh %s is not loaded!", asset.c_str());
	}
	Ref<InstancedMesh> AssetManager::GetInstancedMesh(const std::string& asset)
	{
		if (m_InstancedMesh.find(asset) != m_InstancedMesh.end())
			return m_InstancedMesh[asset].asset;
		else
			LOG_ERR("Instanced mesh %s is not loaded!", asset.c_str());
	}
	Ref<Shader> AssetManager::GetShader(const std::string& asset)
	{
		if (m_Shader.find(asset) != m_Shader.end())
			return m_Shader[asset].asset;
		else
			LOG_ERR("Shader %s is not loaded!", asset.c_str());
	}


	Ref<SkinnedMesh> AssetManager::GetCopyAnimMesh(const std::string& asset)
	{
		if (m_AnimatedMesh.find(asset) != m_AnimatedMesh.end())
			return std::make_shared<SkinnedMesh>(*m_AnimatedMesh[asset].asset);
		else
			LOG_ERR("Animated mesh %s is not loaded!", asset.c_str());
	}

	Ref<Mesh> AssetManager::GetCopyMesh(const std::string& asset)
	{
		if (m_Mesh.find(asset) != m_Mesh.end())
			return std::make_shared <Mesh>(*m_Mesh[asset].asset);
		else
			LOG_ERR("Mesh %s is not loaded!", asset.c_str());
	}

	Ref<InstancedMesh> AssetManager::GetCopyInstancedMesh(const std::string& asset)
	{
		if (m_InstancedMesh.find(asset) != m_InstancedMesh.end())
			return std::make_shared<InstancedMesh>(*m_InstancedMesh[asset].asset);
		else
			LOG_ERR("Instanced mesh %s is not loaded!", asset.c_str());
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