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
void SetupStandardMaterial(StandardMaterial& matData, Material& outMat)
{
	#define PUSH_AND_SET_PROPERTY(MATERIAL, NAME, FORMAT, DATA) MATERIAL.PushProperty(NAME, FORMAT, DATA); MATERIAL.SetProperty(NAME, FORMAT, DATA)

	PUSH_AND_SET_PROPERTY(outMat, "mat_Albedo", MaterialFormat::Float32x3, &matData.mat_Albedo);
	PUSH_AND_SET_PROPERTY(outMat, "mat_Metallic", MaterialFormat::Float32, &matData.mat_Metallic);
	PUSH_AND_SET_PROPERTY(outMat, "mat_Roughness", MaterialFormat::Float32, &matData.mat_Roughness);
}
#endif

CTSI_NAMESPACE_END

#endif // CT_SHADERINTEROP_STANDARD_MATERIAL_H