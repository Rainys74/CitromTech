#version 440

#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
#if HLSLCC_ENABLE_UNIFORM_BUFFERS
#define UNITY_UNIFORM
#else
#define UNITY_UNIFORM uniform
#endif
#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
#if UNITY_SUPPORTS_UNIFORM_LOCATION
#define UNITY_LOCATION(x) layout(location = x)
#define UNITY_BINDING(x) layout(binding = x, std140)
#else
#define UNITY_LOCATION(x)
#define UNITY_BINDING(x) layout(std140)
#endif
precise vec4 u_xlat_precise_vec4;
precise ivec4 u_xlat_precise_ivec4;
precise bvec4 u_xlat_precise_bvec4;
precise uvec4 u_xlat_precise_uvec4;
layout(location = 0) uniform 	mat4x4 _19_u_MVP;
layout(location = 0) in  vec4 in_POSITION0;
layout(location = 1) in  vec2 in_NORMAL0;
layout(location = 0) out vec2 vs_TEXCOORD1;
vec4 u_xlat0;
void main()
{
    //MOV
    vs_TEXCOORD1.xy = in_NORMAL0.xy;
    //MUL
    u_xlat0 = in_POSITION0.yyyy * matricesVS._19_u_MVP[1];
    //MAD
    u_xlat0 = in_POSITION0.xxxx * matricesVS._19_u_MVP[0] + u_xlat0;
    //MAD
    u_xlat0 = in_POSITION0.zzzz * matricesVS._19_u_MVP[2] + u_xlat0;
    //MAD
    gl_Position = in_POSITION0.wwww * matricesVS._19_u_MVP[3] + u_xlat0;
    //RET
    return;
}
