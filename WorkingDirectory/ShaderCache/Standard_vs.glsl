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
layout(location = 0) uniform 	mat4x4 transform;
layout(location = 0) in  vec3 in_Position0;
vec3 u_xlat0;
void main()
{
    //MOV
    gl_Position.zw = vec2(0.0, 1.0);
    //MOV
    u_xlat0.xy = in_Position0.xy;
    //MOV
    u_xlat0.z = 1.0;
    //DP3
    gl_Position.x = dot(u_xlat0.xyz, CBufVS.transform[0].xyw);
    //DP3
    gl_Position.y = dot(u_xlat0.xyz, CBufVS.transform[1].xyw);
    //RET
    return;
}
