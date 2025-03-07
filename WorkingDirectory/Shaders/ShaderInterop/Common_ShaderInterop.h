#ifndef CT_SHADERINTEROP_COMMON_H
#define CT_SHADERINTEROP_COMMON_H

#include "Core_ShaderInterop.h"

#define CTSI_CBSLOT_MATRICES 0
#define CTSI_CBSLOT_LIGHTING 1
#define CTSI_CBSLOT_MATERIAL 2

CTSI_CONSTANT_BUFFER(Material, CTSI_CBSLOT_MATERIAL) // just for demonstration purposes
{
	float u_Test;
	CTSI_TYPE_COLOR4 u_ColorData;
};

#endif // CT_SHADERINTEROP_COMMON_H