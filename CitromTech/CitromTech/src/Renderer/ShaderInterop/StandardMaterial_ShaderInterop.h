#ifndef CT_SHADERINTEROP_STANDARD_MATERIAL_H
#define CT_SHADERINTEROP_STANDARD_MATERIAL_H

#include "Common_ShaderInterop.h"

CTSI_NAMESPACE_BEGIN

CTSI_CONSTANT_BUFFER(StandardMaterial, CTSI_CBSLOT_MATERIAL)
{
	//CTSI_TYPE_COLOR3 materialAlbedo;
};

#ifdef CTSI_LANGUAGE_CPP
#include "Renderer/MaterialSystem.h"
void SetupStandardMaterial(StandardMaterial& matData, Material& outMat)
{
}
#endif

CTSI_NAMESPACE_END

#endif // CT_SHADERINTEROP_STANDARD_MATERIAL_H