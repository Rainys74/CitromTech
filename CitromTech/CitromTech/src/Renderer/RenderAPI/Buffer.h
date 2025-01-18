#pragma once

#include "Graphics.h"
#include "Shader.h"

#include "CTL/RefPointer.h"
#include "CTL/DArray.h"

namespace Citrom::RenderAPI
{
	// Vertex Buffer
	struct VertexBufferLayoutObject
	{
		////unsigned int itemID;			// 0 - position, 1 - texture coordinates. OpenGL: index, DirectX11 requires string name Maybe use uintptr_t for a const char* dx11 and uint32_t opengl?
		//unsigned int dataCount;			// 3 for position, 2 for texture coordinates. OpenGL: size
		//unsigned int bytesPerVertex;	// 3+2=5*sizeof(float) = the amount of bytes of data for a single vertex. OpenGL: stride
		//void* bytesPerItemStart;		// starting position for the item id, 0 - 3 * sizeof(float), 1 - 2 * sizeof(float) or just use offsetof(firstItem, secondItem). OpenGL: pointer
		//unsigned char normalized;		// GL_FALSE. OpenGL: normalized
		//int dataType;					// GL_FLOAT, GL_UNSIGNED_INT. OpenGL: type
	};

	struct VertexBufferLayoutDesc
	{
		const Shader* shader;
		CTL::DArray<VertexBufferLayoutObject> layoutElements;

		void PushLayout()
		{
			layoutElements.PushBack(VertexBufferLayoutObject{});
		}
	};

	struct VertexBufferLayout
	{
		VertexBufferLayoutDesc descriptor;
		CTL::Ref<void> internal;
	};

	struct VertexBufferDesc
	{
		const void* data;
		size_t size;
		Usage usage;
	};

	struct VertexBuffer
	{
		VertexBufferDesc descriptor;
		CTL::Ref<void> internal;
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