#pragma once
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/Renderer/Texture.h"

#include "Renderable2D.h"

namespace Dot {
	class Renderer2D
	{
	public:
		Renderer2D(unsigned int batchSize);

		void ClearBuffer();
		void ClearBufferOffset(int offset,int size);
		void RestartBuffer();
		void Push(const QuadVertex* data,int len);
		void PushOffset(const QuadVertex* data, int len, int offsetElements=0);
		void PrepareForRender();
		void BeginScene(const Ref<Shader>& shader, const Ref<OrthoCamera>& camera);
		void Render();
		void EndScene();

	private:
		Ref<ArrayBuffer> m_VAO;
		unsigned int m_BatchSize;

		Vertex* m_Buffer;
		unsigned int m_NumDataStored = 0;
	
	};
}