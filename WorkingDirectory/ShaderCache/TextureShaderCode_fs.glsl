#version 440

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
UNITY_LOCATION(0) uniform  sampler2D u_Texture;
layout(location = 0) in  vec2 vs_TEXCOORD1;
layout(location = 0) out vec4 SV_Target0;
void main()
{
    SV_Target0 = texture(u_Texture, vs_TEXCOORD1.xy);
    return;
}
