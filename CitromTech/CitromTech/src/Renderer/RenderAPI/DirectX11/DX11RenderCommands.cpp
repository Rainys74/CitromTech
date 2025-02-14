#include "DX11Device.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

namespace Citrom::RenderAPI
{
	CommandBuffer DX11Device::s_RenderCommandBuffer;

	void DX11Device::RCBegin()
	{
		BeginCommandBuffer(&s_RenderCommandBuffer);
	}
	void DX11Device::RCEnd()
	{
		SubmitCommandBuffer(&s_RenderCommandBuffer);
		ResetCommandBuffer(&s_RenderCommandBuffer); // TODO: is this needed?
	}

	void DX11Device::RCClearColor(float32 r, float32 g, float32 b, float32 a)
	{
		const float colors[] = { r, g, b, a };
		DXCall(m_DeviceContext->ClearRenderTargetView(m_RenderTarget, colors));
		// TODO: is this ok here?
		DXCall(m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0));
	}
	void DX11Device::RCDrawIndexed(uint32 indexCount, uint32 startIndex, int32 baseVertexLocation)
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
	void DX11Device::RCDraw(uint32 vertexCount, uint32 startVertexLocation)
	{
		//glDrawArrays(GL_TRIANGLES, 0 /*first*/, vertex_count);
		m_DeviceContext->Draw(vertexCount, startVertexLocation);
	}
}
#endif