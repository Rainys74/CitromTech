#ifndef CT_SHADERINTEROP_LIGHTING_H
#define CT_SHADERINTEROP_LIGHTING_H

#include "Common_ShaderInterop.h"

#define MAX_DIRECTIONAL_LIGHTS (2)
#define MAX_POINT_LIGHTS (100)

CTSI_NAMESPACE_BEGIN

struct alignas(16) BaseLight
{
    CTSI_TYPE_COLOR3 color;
    float intensity;
};

struct alignas(16) SkyLight
{
    BaseLight base;
};

struct alignas(16) DirectionalLight
{
    BaseLight base;
    CTSI_TYPE_FLOAT3 direction;
    // TODO: you can add 1 byte of stuff here to not waste on alignment
    float padding1;
};

CTSI_CONSTANT_BUFFER(Lighting, CTSI_CBSLOT_LIGHTING)
{
    DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
    CTSI_TYPE_UINT directionalLightCount;

    SkyLight skyLight;
};

CTSI_NAMESPACE_END

#ifdef CTSI_LANGUAGE_HLSL
// TODO: do i need to move this to another hlsl file?




float4 CalcLightInternal_Standard(BaseLight baseLight, float3 lightDirection, float3 normal)
{
    return float4(0, 0, 0, 0);
}

float4 CalcDirectionalLight_Standard(int index, float3 normal)
{
    return float4(0, 0, 0, 0);
}
#endif

#endif // CT_SHADERINTEROP_LIGHTING_H