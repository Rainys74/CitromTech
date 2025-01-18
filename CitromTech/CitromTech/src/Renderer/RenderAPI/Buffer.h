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
		//std::string itemName;			// DirectX11: SemanticName
		//unsigned int itemID;			// 0 - position, 1 - texture coordinates. OpenGL: index, DirectX11: SemanticIndex
		//unsigned int dataCount;		// 3 for position, 2 for texture coordinates. OpenGL: size
		//unsigned int bytesPerVertex;	// 3+2=5*sizeof(float) = the amount of bytes of data for a single vertex. OpenGL: stride
		//void* bytesPerItemStart;		// starting position for the item id, 0 - 3 * sizeof(float), 1 - 2 * sizeof(float) or just use offsetof(firstItem, secondItem). OpenGL: pointer
		//unsigned char normalized;		// GL_FALSE. OpenGL: normalized
		//int dataType;					// GL_FLOAT, GL_UNSIGNED_INT. OpenGL: type

		std::string elementName;
		uint32 elementID;

		Format elementFormat;

		//size_t bytesPerVertex; // Stride
	};

	struct VertexBufferLayoutDesc
	{
		const Shader* shader;
		CTL::DArray<VertexBufferLayoutObject> layoutElements;

		void PushLayout(const VertexBufferLayoutObject& layoutObj)
		{
			layoutElements.PushBack(layoutObj);
		}
		void PushLayout(std::string elementName, uint32 elementID, Format elementFormat)
		{
			layoutElements.PushBack(VertexBufferLayoutObject{elementName, elementID, elementFormat});
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
		Format format; // Vertex Data position format
		// TODO: since position format isn't going to cut it long term
		// consider adding a pointer to vb layouts over here
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