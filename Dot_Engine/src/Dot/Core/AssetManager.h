#pragma once
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/AnimatedMesh.h"
#include "Dot/Renderer/Mesh.h"
#include "Dot/Renderer/Shaders/Shader.h"
#include "Dot/Renderer/Camera/Camera.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"

namespace Dot {

	// Very simple Asset Manager
	class AssetManager
	{
	public:
		AssetManager();
		void LoadAssets(const std::string& file);
		
		void PushTexture(const std::string& name, Ref<Texture2D> asset);
		void PushCubeMap(const std::string& name, Ref<CubeMapTexture> asset);
		void PushAnimatedMesh(const std::string& name, Ref<AnimatedMesh> asset);
		void PushMesh(const std::string& name, Ref<Mesh> asset);
		void PushInstancedMesh(const std::string& name, Ref<InstancedMesh> asset);
		void PushShader(const std::string& name, Ref<Shader> asset);

		Ref<Texture2D>		GetTexture(const std::string& asset);
		Ref<CubeMapTexture> GetCubeMap(const std::string& asset);
		Ref<AnimatedMesh>	GetAnimMesh(const std::string& asset);
		Ref<Mesh>			GetMesh(const std::string& asset);
		Ref<InstancedMesh>  GetInstancedMesh(const std::string& asset);
		Ref<Shader>			GetShader(const std::string& asset);
	
		static Scope<AssetManager>& Get() { return s_Instance; }
	private:
		std::vector<std::string> TokenizeString(const std::string& str, const std::string& delimiter);
	private:
		std::string m_AssetFile;

	private:
		std::unordered_map<std::string, Ref<Texture2D> >	   m_Texture;
		std::unordered_map<std::string, Ref<CubeMapTexture> >  m_CubeMap;
		std::unordered_map<std::string, Ref<AnimatedMesh> >	   m_AnimatedMesh;
		std::unordered_map<std::string, Ref<Mesh> >			   m_Mesh;
		std::unordered_map<std::string, Ref<InstancedMesh>>    m_InstancedMesh;
		std::unordered_map<std::string, Ref<Shader> >		   m_Shader;

	private:
		static Scope<AssetManager> s_Instance;



	private:
		struct TextureAsset
		{
			std::string name;
			std::string path;
			bool filters;
			bool flipped;
		};
		struct ShaderAsset
		{
			std::string name;
			std::string path;
		};
		struct CubeMapAsset
		{
			std::string name;
			std::vector<std::string>face;
		};
		struct AnimatedMeshAsset
		{
			std::string name;
			std::string path;
		};
		struct InstancedMeshAsset
		{
			std::string name;
			std::string path;
			unsigned int capacity;
		};
		struct MeshAsset
		{
			std::string name;
			std::string path;
		};
	};
}