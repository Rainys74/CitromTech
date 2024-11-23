#pragma once

#include "Graphics.h"

#include "VertexBuffer.h"

namespace Citrom::RenderAPI
{
	class Device
	{
	public:
		// Vertex Buffer
		// TODO: figure out whether to return or set using pointers
		VertexBuffer CreateVertexBuffer(VertexBufferDesc* descriptor);

		// Render Commands
		//virtual void RCDrawIndexed() = 0;
		//virtual void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) = 0;
	private:
	};
}