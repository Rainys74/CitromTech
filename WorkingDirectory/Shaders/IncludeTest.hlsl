#include "IncludeHeaderTest.hhlsl"

//#include "Common_ShaderInterop.h"
#include <ShaderInterop/Common_ShaderInterop.h>

#ifdef IF_THIS_IS_DEFINED_WORKS

float4 vsmain(float3 pos : Position) : SV_Position
{
    return float4(pos, 1.0);
}
float4 psmain() : SV_Target
{
    VECTOR3 thisisavec3test = VECTOR3(0.5, 1.0, 0.74);
    //return float4(1.0, 1.0, 1.0, 1.0);
    return float4(thisisavec3test + CTSI_TYPE_FLOAT3(u_Test, 0.0f, 0.0f), 1.0f);
}

#endif