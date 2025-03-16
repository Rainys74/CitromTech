#version 440

precise vec4 u_xlat_precise_vec4;
precise ivec4 u_xlat_precise_ivec4;
precise bvec4 u_xlat_precise_bvec4;
precise uvec4 u_xlat_precise_uvec4;
layout(location = 0) in  vec3 in_Position0;
void main()
{
    gl_Position.xyz = in_Position0.xyz;
    gl_Position.w = 1.0;
    return;
}
