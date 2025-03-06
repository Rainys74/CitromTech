#include "IncludeHeaderTest.hhlsl"

#ifdef IF_THIS_IS_DEFINED_WORKS

float4 vsmain(float3 pos : Position)
{
    return float4(pos, 1.0);
}
float4 psmain()
{
    VECTOR3 thisisavec3test = VECTOR3(0.5, 1.0, 0.74);
    //return float4(1.0, 1.0, 1.0, 1.0);
    return float4(thisisavec3test, 1.0f);
}

#endif