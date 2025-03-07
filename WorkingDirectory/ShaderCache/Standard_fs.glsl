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
uniform 	vec3 cameraLocalPos;
struct base_Type {
	vec3 color;
	float intensity;
};
struct skyLight_Type {
	base_Type base;
};
struct directionalLights_Type {
	base_Type base;
	vec3 direction;
};
layout(location = 1) uniform 	skyLight_Type skyLight;
uniform 	directionalLights_Type directionalLights[2];
UNITY_LOCATION(0) uniform  sampler2D tex;
layout(location = 0) in  vec2 vs_TexCoord0;
layout(location = 1) in  vec3 vs_Normal0;
layout(location = 2) in  vec3 vs_LocalPosition0;
layout(location = 0) out vec4 SV_Target0;
vec4 u_xlat0;
ivec3 u_xlati0;
bool u_xlatb0;
vec4 u_xlat1;
bool u_xlatb1;
vec4 u_xlat2;
vec3 u_xlat3;
float u_xlat9;
void main()
{
    //ADD
    u_xlat0.xyz = (-vs_LocalPosition0.xyz) + MatricesPS.cameraLocalPos.xyz;
    //DP3
    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
    //RSQ
    u_xlat9 = inversesqrt(u_xlat9);
    //MAD
    u_xlat0.xyz = u_xlat0.xyz * vec3(u_xlat9) + (-LightingPS.directionalLights[0].direction.xyz);
    //DP3
    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
    //RSQ
    u_xlat9 = inversesqrt(u_xlat9);
    //MUL
    u_xlat0.xyz = vec3(u_xlat9) * u_xlat0.xyz;
    //DP3
    u_xlat0.x = dot(vs_Normal0.xyz, u_xlat0.xyz);
    //LOG
    u_xlat3.x = log2(abs(u_xlat0.x));
    //LT
    u_xlatb0 = 0.0<u_xlat0.x;
    //MUL
    u_xlat3.x = u_xlat3.x * 255.0;
    //EXP
    u_xlat3.x = exp2(u_xlat3.x);
    //MUL
    u_xlat3.xyz = u_xlat3.xxx * vec3(1.0, 0.0, 0.0);
    //AND
    u_xlati0.xyz = ivec3((uvec3(bvec3(u_xlatb0)) * 0xFFFFFFFFu) & floatBitsToUint(u_xlat3.xyz));
    //DP3
    u_xlat9 = dot(vs_Normal0.xyz, vs_Normal0.xyz);
    //RSQ
    u_xlat9 = inversesqrt(u_xlat9);
    //MUL
    u_xlat1.xyz = vec3(u_xlat9) * vs_Normal0.xyz;
    //DP3
    u_xlat9 = dot(u_xlat1.xyz, (-LightingPS.directionalLights[0].direction.xyz));
    //LT
    u_xlatb1 = 0.0<u_xlat9;
    //AND
    u_xlat9 = u_xlatb1 ? u_xlat9 : float(0.0);
    //AND
    u_xlat1.xyz = bool(u_xlatb1) ? intBitsToFloat(u_xlati0.xyz) : vec3(0.0, 0.0, 0.0);
    //ADD
    u_xlat2.w = u_xlat9 + u_xlat1.x;
    //ADD
    u_xlat2.xyz = vec3(u_xlat9) + LightingPS.skyLight.base.color.xyz;
    //MOV
    u_xlat1.w = 1.0;
    //ADD
    u_xlat0 = u_xlat1 + u_xlat2;
    u_xlat0 = clamp(u_xlat0, 0.0, 1.0);
    //SAMPLE
    u_xlat1 = texture(tex, vs_TexCoord0.xy);
    //MUL
    SV_Target0 = u_xlat0 * u_xlat1;
    //RET
    return;
}
