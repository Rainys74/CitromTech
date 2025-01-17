#include "DX11Device.h"
#include "Renderer/RenderAPI/Buffer.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

namespace Citrom::RenderAPI
{
	struct VertexBufferDX11
	{
		WRL::ComPtr<ID3D11Buffer> buffer;
	};

	VertexBuffer DX11Device::CreateVertexBuffer(VertexBufferDesc* descriptor) 
	{ 
		return VertexBuffer(); 
	}
	void DX11Device::BindVertexBuffer(VertexBuffer* vb)
	{

	}

	struct IndexBufferDX11
	{
		WRL::ComPtr<ID3D11Buffer> buffer;
	};

	IndexBuffer DX11Device::CreateIndexBuffer(IndexBufferDesc* descriptor)
	{
		IndexBuffer ib;
		ib.internal = CTL::CreateRef<IndexBufferDX11>();
		auto internalData = static_cast<IndexBufferDX11*>(ib.internal.get());
		ib.descriptor = *descriptor;

		//internalData->count = descriptor->count;

		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT; // use descriptor->usage
		ibd.CPUAccessFlags = 0;
		if (ibd.Usage == D3D11_USAGE_DYNAMIC)
			ibd.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		ibd.MiscFlags = 0;
		ibd.ByteWidth = descriptor->count * sizeof(uint32);
		//ibd.StructureByteStride = sizeof(uint32);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = descriptor->data;

		HRESULT hr;
		DXCallHR(m_Device->CreateBuffer(&ibd, &isd, &internalData->buffer));

		// Bind index buffer
		DXCall(m_DeviceContext->IASetIndexBuffer(internalData->buffer.Get(), DXGI_FORMAT_R32_UINT, 0));

		return ib;
	}
	void DX11Device::BindIndexBuffer(IndexBuffer* ib)
	{
		auto internalData = static_cast<IndexBufferDX11*>(ib->internal.get());

		// Bind index buffer
		DXCall(m_DeviceContext->IASetIndexBuffer(internalData->buffer.Get(), DXGI_FORMAT_R32_UINT, 0));
	}
}
#endif