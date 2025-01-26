#include "DX11Device.h"
#include "Renderer/RenderAPI/Buffer.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"
#include "CitromAssert.h"

namespace Citrom::RenderAPI
{
	struct VertexBufferDX11
	{
		WRL::ComPtr<ID3D11Buffer> buffer;
	};
	struct VertexBufferLayoutDX11
	{
		WRL::ComPtr<ID3D11InputLayout> inputLayout;
	};

	VertexBuffer DX11Device::CreateVertexBuffer(VertexBufferDesc* descriptor) 
	{ 
		CREATE_BUFFER_INTERNAL(VertexBuffer, VertexBufferDX11, vb, internalData);

		// Create vertex buffer
		D3D11_BUFFER_DESC vbd = {};
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.Usage = UsageToD3D11Usage(descriptor->usage);
		vbd.CPUAccessFlags = 0;
		if (vbd.Usage == D3D11_USAGE_DYNAMIC)
			vbd.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags = 0;
		vbd.ByteWidth = descriptor->size * sizeof(float);
		vbd.StructureByteStride = GetLayoutStride(descriptor->vbLayoutDesc);

		D3D11_SUBRESOURCE_DATA vsd = {};
		vsd.pSysMem = descriptor->data;

		HRESULT hr;
		DXCallHR(m_Device->CreateBuffer(&vbd, &vsd, &internalData->buffer));

		return vb;
	}
	void DX11Device::BindVertexBuffer(VertexBuffer* vb)
	{
		// // TODO: remove this since vertex buffer needs layout information to get bound, so it will most likely get bound in BindVertexBufferLayout
		auto internalData = static_cast<VertexBufferDX11*>(vb->internal.get());

		// Bind Vertex Buffer to pipeline
		const UINT stride = GetLayoutStride(vb->descriptor.vbLayoutDesc);
		const UINT offset = 0;
		DXCall(m_DeviceContext->IASetVertexBuffers(0, 1, internalData->buffer.GetAddressOf(), &stride, &offset));
	}
	WRL::ComPtr<ID3D11Buffer> DX11GetVertexBuffer(const VertexBuffer* vb)
	{
		auto internalData = static_cast<VertexBufferDX11*>(vb->internal.get());

		return internalData->buffer;
	}

	VertexBufferLayout DX11Device::CreateVertexBufferLayout(VertexBufferLayoutDesc* descriptor)
	{
		VertexBufferLayout vbLayout;
		vbLayout.internal = CTL::CreateRef<VertexBufferLayoutDX11>();
		auto internalData = static_cast<VertexBufferLayoutDX11*>(vbLayout.internal.get());
		//vbLayout.descriptor = *descriptor; // caused a hard to debug error.. realized it's because CANNOT COPY DESCRIPTOR IF IT INCLUDES ALLOCATED MEMORY SUCH AS DARRAY IN THIS CASE!

		// input (vertex) layout
		CTL::DArray<D3D11_INPUT_ELEMENT_DESC> ieds;
		UINT offset = 0; // also known as pointer in OpenGL
		for (auto& element : descriptor->layoutElements)
		{
			D3D11_INPUT_ELEMENT_DESC ied = {};
			ied.SemanticName = element.elementName.c_str();
			ied.SemanticIndex = element.elementID;
			ied.Format = FormatToDXGIFormat(element.elementFormat);
			ied.InputSlot = 0; // not important
			ied.AlignedByteOffset = offset; // offset between the first item and the current in bytes

			offset += GetFormatSize(element.elementFormat);

			// Instancing stuff (not important)
			ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			ied.InstanceDataStepRate = 0;

			ieds.PushBack(ied);
		}
		//D3D11_INPUT_ELEMENT_DESC ied = {};
		//ied.SemanticName = "Position";
		//ied.SemanticIndex = 0;
		//ied.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		//ied.InputSlot = 0; // not important
		//ied.AlignedByteOffset = 0;
		//// Instancing stuff (not important)
		//ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		//ied.InstanceDataStepRate = 0;

		WRL::ComPtr<ID3DBlob> blob = DX11GetVertexShaderBlob(descriptor->shader);

		HRESULT hr;
		DXCallHR(m_Device->CreateInputLayout(ieds.Data(), ieds.Count(), blob->GetBufferPointer(), blob->GetBufferSize(), &internalData->inputLayout));

		return vbLayout;
	}
	void DX11Device::BindVertexBufferLayout(VertexBufferLayout* vbLayout)
	{
		auto internalData = static_cast<VertexBufferLayoutDX11*>(vbLayout->internal.get());

		// Bind Vertex Layout
		DXCall(m_DeviceContext->IASetInputLayout(internalData->inputLayout.Get()));
	}

	struct IndexBufferDX11
	{
		WRL::ComPtr<ID3D11Buffer> buffer;
	};

	IndexBuffer DX11Device::CreateIndexBuffer(IndexBufferDesc* descriptor)
	{
		CREATE_BUFFER_INTERNAL(IndexBuffer, IndexBufferDX11, ib, internalData);

		//internalData->count = descriptor->count;

		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = UsageToD3D11Usage(descriptor->usage);
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

	struct UniformBufferDX11
	{
		WRL::ComPtr<ID3D11Buffer> buffer;
	};

	UniformBuffer DX11Device::CreateUniformBuffer(UniformBufferDesc* descriptor)
	{
		CREATE_BUFFER_INTERNAL(UniformBuffer, UniformBufferDX11, ub, internalData);

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = UsageToD3D11Usage(descriptor->usage);
		cbd.CPUAccessFlags = 0;
		if (cbd.Usage == D3D11_USAGE_DYNAMIC)
			cbd.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0x00000000;
		cbd.ByteWidth = descriptor->dataBytes;
		cbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = descriptor->data;

		HRESULT hr;
		DXCallHR(m_Device->CreateBuffer(&cbd, &csd, &internalData->buffer));

		return ub;
	}
	void DX11Device::BindUniformBuffer(UniformBuffer* ub)
	{
		GET_BUFFER_INTERNAL(UniformBufferDX11, ub, internalData);

		DXCall(m_DeviceContext->VSSetConstantBuffers(0, 1, internalData->buffer.GetAddressOf()));
	}
	void DX11Device::SetUniformBufferData(UniformBuffer* ub, const void* data, const size_t size)
	{
		GET_BUFFER_INTERNAL(UniformBufferDX11, ub, internalData);

		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE msr;
		DXCallHR(m_DeviceContext->Map(internalData->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0x00000000, &msr));

		Memory::Copy(msr.pData, data, size);

		DXCall(m_DeviceContext->Unmap(internalData->buffer.Get(), 0));
	}
}
#endif