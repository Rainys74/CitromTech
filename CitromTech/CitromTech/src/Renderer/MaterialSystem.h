#pragma once

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
		//std::string name;
		MaterialFormat propertyFormat;
		void* data;
	};

	class Material
	{
	public:

	private:
		std::string m_ShaderName; // TODO: shader reference or shader name?
		// CTL::StdStrHashMap<MaterialProperty> m_Properties; CTL::DArray<MaterialProperty> properties;
	};
	struct MaterialInstance
	{
		Material* mat;
		Material values;
	};
}