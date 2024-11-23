#pragma once

#include "Graphics.h"

#include "CTL/RefPointer.h"
#include "CTL/DArray.h"

namespace Citrom::RenderAPI
{
	struct VertexBufferLayoutObject
	{

	};

	struct VertexBufferDesc
	{
		const void* data;
		uint32 size;
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
}