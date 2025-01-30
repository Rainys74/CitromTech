#pragma once

#include "RenderAPI/GraphicsDevice.h"

#include "CTL/String.h"
#include "CTL/HashMap.h"

namespace Citrom
{
	enum class MaterialFormat
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
		void* data; // Ref or copy data?
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
		CTL::DArray<MaterialProperty> m_Properties; // TODO: maybe separate metadata and buffer data to not handle memory myself?

		RenderAPI::Device* m_Device;
	};
	struct MaterialInstance
	{
		//Material* mat;
		//Material values;
	};
}