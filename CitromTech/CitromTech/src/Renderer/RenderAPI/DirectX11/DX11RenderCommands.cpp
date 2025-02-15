#include "DX11Device.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

#ifdef CT_DEBUG
#include <d3d9.h> // D3DPERF_BeginEvent, D3DPERF_EndEvent, D3DCOLOR_XRGB
#pragma comment(lib, "d3d9.lib")
#endif

namespace Citrom::RenderAPI
{
	/*CommandBuffer DX11Device::s_RenderCommandBuffer;

	void DX11Device::RCBegin()
	{
		BeginCommandBuffer(&s_RenderCommandBuffer);
	}
	void DX11Device::RCEnd()
	{
		SubmitCommandBuffer(&s_RenderCommandBuffer);
		ResetCommandBuffer(&s_RenderCommandBuffer); // TODO: is this needed?
	}*/

	void DX11Device::RCClearColor(float32 r, float32 g, float32 b, float32 a)
	{
		const float colors[] = { r, g, b, a };
		DXCall(m_DeviceContext->ClearRenderTargetView(m_RenderTarget, colors));
		// TODO: is this ok here?
		DXCall(m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0));
	}
	void DX11Device::RCDrawIndexed(uint32 indexCount, uint32 startIndex, int32 baseVertexLocation, CommandBuffer* cmd)
	{
		//glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(startIndex * sizeof(GLuint)), baseVertex);
		// or
		//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(startIndex * sizeof(GLuint)));
		//vkCmdDrawIndexed(commandBuffer, indexCount, 1, startIndex, baseVertex, instanceOffset);
		/*
		* [encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                   indexCount:indexCount
                    indexType:MTLIndexTypeUInt32
                  indexBuffer:indexBuffer
            indexBufferOffset:startIndex * sizeof(uint32_t)
                    instanceCount:1
                   baseVertex:baseVertex];
		*/
		DXCall(m_DeviceContext->DrawIndexed(indexCount, startIndex, baseVertexLocation));
	}
	void DX11Device::RCDraw(uint32 vertexCount, uint32 startVertexLocation, CommandBuffer* cmd)
	{
		//glDrawArrays(GL_TRIANGLES, 0 /*first*/, vertex_count);
		m_DeviceContext->Draw(vertexCount, startVertexLocation);
	}

#ifdef CT_DEBUG
	static D3DCOLOR GenerateColorFromName(const char* name)
	{
		uint32 hash = 2166136261u; // FNV-1a hash seed
		while (*name)
		{
			hash ^= (uint8)(*name++);
			hash *= 16777619u;
		}

		// Extract RGB components from the hash
		uint8 r = (hash >> 16) & 0xFF;
		uint8 g = (hash >> 8) & 0xFF;
		uint8 b = hash & 0xFF;

		// Ensure the color is not too dark or too bright
		if (r + g + b < 128) { r += 100; g += 100; b += 100; }
		if (r + g + b > 600) { r /= 2; g /= 2; b /= 2; }

		return D3DCOLOR_XRGB(r, g, b);
	}
#endif
	void DX11Device::RCPushDebugGroup(const char* name, CommandBuffer* cmd)
	{
		//D3DPERF_BeginEvent // DirectX 9
		//PIXBeginEvent // DirectX 12

		// DirectX 11.1
		//static WRL::ComPtr<ID3DUserDefinedAnnotation> annotation;
		//if (!annotation || !annotation->GetStatus())
		// 
		//context->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), reinterpret_cast<void**>(annotation.GetAddressOf()));
		// Or
		//context.As(&annotation);

		//annotation->BeginEvent(wideName);
#ifdef CT_DEBUG
		std::wstring wideStr(name, name + std::strlen(name));
		//D3DPERF_BeginEvent(D3DCOLOR_XRGB(255, 0, 0), wideStr.c_str()); // Red, Green, Yellow or Blue?
		D3DPERF_BeginEvent(GenerateColorFromName(name), wideStr.c_str()); // We're using DirectX 9 because for now i want to support D3D11.0
#endif
	}
	void DX11Device::RCPopDebugGroup(CommandBuffer* cmd)
	{
#ifdef CT_DEBUG
		D3DPERF_EndEvent();
#endif
	}
}
#endif