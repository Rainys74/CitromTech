#pragma once

#include "RenderAPI/GraphicsDevice.h"

#include "CTL/String.h"
#include "CTL/HashMap.h"

namespace Citrom
{
	/*enum class MaterialFormat
	{
		Float32,
		Float32x3,
		Float32x4,
		Float32x4x4,
		Int32,
		UInt32
	};
	constexpr size_t GetMaterialFormatSize(MaterialFormat format);

	struct MaterialProperty
	{
		std::string name;
		MaterialFormat propertyFormat;
		void* dataPtr; // Ref or copy data?, right now only points to the DArray
	};

	class Material
	{
	public:
		Material(const std::string& shaderName = "Standard");
		~Material();

		void PushProperty(const std::string& name, const MaterialFormat format, const void* propertyData);
		void UpdateData(const std::string& name, const void* newData, const MaterialFormat format);
		void Render();

		FORCE_INLINE RenderAPI::Shader* GetShader() { return &m_Shader; };
		FORCE_INLINE RenderAPI::UniformBuffer* GetUniformBuffer() { return &m_UniformBuffer; };
	private:
		MaterialProperty* GetPropertyByName(const std::string& name);
	private:
		RenderAPI::Shader m_Shader;
		RenderAPI::UniformBuffer m_UniformBuffer;

		//CTL::StdStrHashMap<MaterialProperty> m_Properties; CTL::DArray<MaterialProperty> properties;
		CTL::DArray<MaterialProperty> m_Properties;
		CTL::DArray<uint8> m_BufferData;

		RenderAPI::Device* m_Device;
	};
	struct MaterialInstance
	{
		//Material* mat;
		//Material values;
	};*/

#define MATERIAL_BUFFER_CLASS(NAME)

	template<typename CB>
	class Material
	{
	public:
		Material(const std::string& shaderName = "Standard")
			: m_Device(RenderAPI::Device::Get())
		{
			RenderAPI::ShaderDesc sd = {};
			sd.name = shaderName;

			m_Shader = m_Device->CreateShader(&sd);

			RenderAPI::UniformBufferDesc ubd = {};
			ubd.data = &m_CBData;
			ubd.dataBytes = sizeof(CB);
			ubd.usage = RenderAPI::Usage::Dynamic;

			m_UniformBuffer = m_Device->CreateUniformBuffer(&ubd);
		}
		~Material() = default;

		void Bind()
		{
			m_Device->BindShader(&m_Shader);
			m_Device->BindUniformBuffer(&m_UniformBuffer, RenderAPI::ShaderType::Vertex, 1);
		}
		void UpdateData()
		{
			Bind();
			m_Device->SetUniformBufferData(&m_UniformBuffer, &m_CBData, sizeof(CB));
		}

		FORCE_INLINE void SetBufferData(const CB& newBuffer) { m_CBData = newBuffer; }
		FORCE_INLINE CB* GetBufferData() { return &m_CBData; }
		FORCE_INLINE constexpr size_t GetBufferSize() { return sizeof(CB); }
	private:
		CB m_CBData;

		RenderAPI::Shader m_Shader;
		RenderAPI::UniformBuffer m_UniformBuffer;

		RenderAPI::Device* m_Device;
	};
}