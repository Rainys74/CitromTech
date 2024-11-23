#pragma once

#include "Graphics.h"

#include "Buffer.h"

namespace Citrom::RenderAPI
{
	class Device
	{
	public:
		// Vertex Buffer
		// TODO: figure out whether to return or set using pointers
		virtual VertexBuffer CreateVertexBuffer(VertexBufferDesc* descriptor) = 0;
		virtual void BindVertexBuffer(VertexBuffer* vb) = 0;

		virtual IndexBuffer CreateIndexBuffer(IndexBufferDesc* descriptor) = 0;
		virtual void BindIndexBuffer(IndexBuffer* ib) = 0;

		// Render Commands
		virtual void RCDrawIndexed(uint32 indexCount) = 0;
		virtual void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) = 0;
	private:
	};
}