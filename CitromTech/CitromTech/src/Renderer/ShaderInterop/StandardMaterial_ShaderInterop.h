#ifndef CT_SHADERINTEROP_STANDARD_MATERIAL_H
#define CT_SHADERINTEROP_STANDARD_MATERIAL_H

#include "Common_ShaderInterop.h"

CTSI_NAMESPACE_BEGIN

CTSI_CONSTANT_BUFFER(StandardMaterial, CTSI_CBSLOT_MATERIAL)
{
	CTSI_TYPE_COLOR3 mat_Albedo;
	float mat_Metallic; // metalness: 0 - dielectrics, 1 - metals
	float mat_Roughness; // Smoothness = 1 - roughness
};

#ifdef CTSI_LANGUAGE_CPP
#include "Renderer/MaterialSystem.h"

template<bool bPushProperty, bool bSetProperty>
constexpr inline void _PushAndSetProperty(Material& outMat, const std::string& name, const MaterialFormat format, const void* data)
{
	if constexpr(bPushProperty) 
	{
		(outMat).PushProperty(name, format, data);
	} 
	if constexpr(bSetProperty) 
	{
		(outMat).SetProperty(name, format, data);
	}
}

template<bool bPushProperty, bool bSetProperty = true>
inline void TStandardMaterialSetup(const StandardMaterial& matData, Material* outMat)
{
//#define _PUSH_AND_SET_PROPERTY(MATERIAL, NAME, FORMAT, DATA) if constexpr(bPushProperty) { (MATERIAL).PushProperty(NAME, FORMAT, DATA); } if constexpr(bSetProperty) { (MATERIAL).SetProperty(NAME, FORMAT, DATA) }

	_PushAndSetProperty<bPushProperty, bSetProperty>(*outMat, "mat_Albedo", MaterialFormat::Float32x3, &matData.mat_Albedo);
	_PushAndSetProperty<bPushProperty, bSetProperty>(*outMat, "mat_Metallic", MaterialFormat::Float32, &matData.mat_Metallic);
	_PushAndSetProperty<bPushProperty, bSetProperty>(*outMat, "mat_Roughness", MaterialFormat::Float32, &matData.mat_Roughness);
}

void SetupStandardMaterial(const StandardMaterial& matData, Material* outMat)
{
	TStandardMaterialSetup<true, true>(matData, outMat);
}
void SetStandardMaterialData(const StandardMaterial& matData, Material* outMat)
{
	TStandardMaterialSetup<false, true>(matData, outMat);
}
#endif

CTSI_NAMESPACE_END

#endif // CT_SHADERINTEROP_STANDARD_MATERIAL_H