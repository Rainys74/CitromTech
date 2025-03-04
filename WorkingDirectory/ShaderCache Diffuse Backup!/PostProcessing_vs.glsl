#version 440

precise vec4 u_xlat_precise_vec4;
precise ivec4 u_xlat_precise_ivec4;
precise bvec4 u_xlat_precise_bvec4;
precise uvec4 u_xlat_precise_uvec4;
vec2 ImmCB_0_0_2[3];
vec2 ImmCB_0_0_0[3];
layout(location = 0) out vec2 vs_TexCoord0;
int u_xlati0;
void main()
{
    //--- Start Early Main ---
	ImmCB_0_0_2[0] = vec2(0.0, 0.0);
	ImmCB_0_0_2[1] = vec2(2.0, 0.0);
	ImmCB_0_0_2[2] = vec2(0.0, 2.0);
	ImmCB_0_0_0[0] = vec2(-1.0, 1.0);
	ImmCB_0_0_0[1] = vec2(3.0, 1.0);
	ImmCB_0_0_0[2] = vec2(-1.0, -3.0);
    //--- End Early Main ---
    //MOV
    u_xlati0 = gl_VertexID;
    //MOV
    vs_TexCoord0.xy = ImmCB_0_0_2[u_xlati0].xy;
    //MOV
    gl_Position.xy = ImmCB_0_0_0[u_xlati0].xy;
    //MOV
    gl_Position.zw = vec2(0.0, 1.0);
    //RET
    return;
}
