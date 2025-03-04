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
uniform 	vec3 directionalLightDir;
uniform 	vec3 cameraLocalPos;
UNITY_LOCATION(0) uniform  sampler2D tex;
layout(location = 0) in  vec2 vs_TexCoord0;
layout(location = 1) in  vec3 vs_Normal0;
layout(location = 2) in  vec3 vs_LocalPosition0;
layout(location = 0) out vec4 SV_Target0;
vec4 u_xlat0;
bool u_xlatb0;
vec4 u_xlat1;
vec3 u_xlat2;
bool u_xlatb4;
float u_xlat6;
void main()
{
    //DP3
    u_xlat0.x = dot(CBuffer1PS.directionalLightDir.xyz, vs_Normal0.xyz);
    //ADD
    u_xlat0.x = u_xlat0.x + u_xlat0.x;
    //MAD
    u_xlat0.xyz = vs_Normal0.xyz * (-u_xlat0.xxx) + CBuffer1PS.directionalLightDir.xyz;
    //DP3
    u_xlat6 = dot(u_xlat0.xyz, u_xlat0.xyz);
    //RSQ
    u_xlat6 = inversesqrt(u_xlat6);
    //MUL
    u_xlat0.xyz = vec3(u_xlat6) * u_xlat0.xyz;
    //ADD
    u_xlat1.xyz = (-vs_LocalPosition0.xyz) + CBuffer1PS.cameraLocalPos.xyz;
    //DP3
    u_xlat6 = dot(u_xlat1.xyz, u_xlat1.xyz);
    //RSQ
    u_xlat6 = inversesqrt(u_xlat6);
    //MUL
    u_xlat1.xyz = vec3(u_xlat6) * u_xlat1.xyz;
    //DP3
    u_xlat0.x = dot(u_xlat1.xyz, u_xlat0.xyz);
    //LOG
    u_xlat2.x = log2(u_xlat0.x);
    //LT
    u_xlatb0 = 0.0<u_xlat0.x;
    //MUL
    u_xlat2.x = u_xlat2.x * 255.0;
    //EXP
    u_xlat2.x = exp2(u_xlat2.x);
    //AND
    u_xlat0.x = u_xlatb0 ? u_xlat2.x : float(0.0);
    //DP3
    u_xlat2.x = dot(vs_Normal0.xyz, vs_Normal0.xyz);
    //RSQ
    u_xlat2.x = inversesqrt(u_xlat2.x);
    //MUL
    u_xlat2.xyz = u_xlat2.xxx * vs_Normal0.xyz;
    //DP3
    u_xlat0.y = dot(u_xlat2.xyz, (-CBuffer1PS.directionalLightDir.xyz));
    //LT
    u_xlatb4 = 0.0<u_xlat0.y;
    //AND
    u_xlat0.xy = bool(u_xlatb4) ? u_xlat0.xy : vec2(0.0, 0.0);
    //ADD
    u_xlat0.x = u_xlat0.y + u_xlat0.x;
    //ADD
    u_xlat0 = u_xlat0.xxxx + vec4(0.419999987, 0.477999985, 0.626999974, 1.0);
    u_xlat0 = clamp(u_xlat0, 0.0, 1.0);
    //SAMPLE
    u_xlat1 = texture(tex, vs_TexCoord0.xy);
    //MUL
    SV_Target0 = u_xlat0 * u_xlat1;
    //RET
    return;
}
