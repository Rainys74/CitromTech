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
UNITY_BINDING(0) uniform Matrices {
	mat4x4 transform;
	vec3 cameraLocalPos;
};
layout(location = 0) in  vec3 in_Position0;
layout(location = 1) in  vec3 in_Normal0;
layout(location = 2) in  vec2 in_TexCoord0;
layout(location = 0) out vec2 vs_TexCoord0;
layout(location = 1) out vec3 vs_Normal0;
layout(location = 2) out vec3 vs_LocalPosition0;
vec4 u_xlat0;
void main()
{
    vs_TexCoord0.xy = in_TexCoord0.xy;
    u_xlat0.xyz = in_Position0.xyz;
    u_xlat0.w = 1.0;
    gl_Position.x = dot(u_xlat0, transform[0]);
    gl_Position.y = dot(u_xlat0, transform[1]);
    gl_Position.z = dot(u_xlat0, transform[2]);
    gl_Position.w = dot(u_xlat0, transform[3]);
    vs_Normal0.xyz = in_Normal0.xyz;
    vs_LocalPosition0.xyz = in_Position0.xyz;
    return;
}
