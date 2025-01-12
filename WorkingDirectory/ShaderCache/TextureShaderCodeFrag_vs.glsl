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
layout(location = 0) in  vec2 in_NORMAL0;
layout(location = 0) out vec4 vs_TEXCOORD0;
void main()
{
    //SAMPLE_L
    vs_TEXCOORD0 = textureLod(u_Texture, in_NORMAL0.xy, 0.0);
    //RET
    return;
}
