#pragma once

#include "Graphics.h"

#include "CTL/RefPointer.h"
#include "CTL/DArray.h"

namespace Citrom::RenderAPI
{
	// Vertex Buffer
	struct VertexBufferLayoutObject
	{

	};

	struct VertexBufferDesc
	{
		const void* data;
		size_t size;
		Usage usage;
	};

	struct VertexBuffer
	{
		CTL::DArray<VertexBufferLayoutObject> layoutElements;
		VertexBufferDesc descriptor;
		CTL::Ref<void> internal;

		void PushLayout()
		{
			
		}
	};

	// Index Buffer
	struct IndexBufferDesc
	{
		const uint32* data;
		uint32 count;
		Usage usage;
	};

	struct IndexBuffer
	{
		IndexBufferDesc descriptor;
		CTL::Ref<void> internal;

		FORCE_INLINE uint32 GetCount() const { return descriptor.count; }
	};
}