#pragma once
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"

namespace Dot {
	class Renderer2D
	{
	public:
		Renderer2D(unsigned int batchSize);

		void ClearBuffer();
		void ClearBufferOffset(int offset, int size);
		void RestartBuffer();
		void Push(const QuadVertex2D* data, int len);
		void PushOffset(const QuadVertex2D* data, unsigned int len, unsigned int offsetElements = 0);
		void PrepareForRender();
		void BeginScene();
		void Render();
		void EndScene();

	private:
		Ref<ArrayBuffer> m_VAO;
		unsigned int m_BatchSize;

		Vertex2D* m_Buffer;
		unsigned int m_NumDataStored = 0;

	};
}