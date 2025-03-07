#version 440

precise vec4 u_xlat_precise_vec4;
precise ivec4 u_xlat_precise_ivec4;
precise bvec4 u_xlat_precise_bvec4;
precise uvec4 u_xlat_precise_uvec4;
layout(location = 0) out vec4 SV_Target0;
void main()
{
    //MOV
    SV_Target0 = vec4(0.5, 1.0, 0.74000001, 1.0);
    //RET
    return;
}
