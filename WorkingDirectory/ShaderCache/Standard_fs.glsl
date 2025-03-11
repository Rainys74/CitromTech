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
struct base_Type {
	vec3 color;
	float intensity;
};
struct directionalLights_Type {
	base_Type base;
	vec3 direction;
	float padding1;
};
struct skyLight_Type {
	base_Type base;
};
UNITY_BINDING(1) uniform Lighting {
	directionalLights_Type directionalLights[2];
	uint directionalLightCount;
	skyLight_Type skyLight;
};
UNITY_LOCATION(0) uniform  sampler2D tex;
layout(location = 0) in  vec2 vs_TexCoord0;
layout(location = 1) in  vec3 vs_Normal0;
layout(location = 2) in  vec3 vs_LocalPosition0;
layout(location = 0) out vec4 SV_Target0;
vec4 u_xlat0;
ivec4 u_xlati0;
bool u_xlatb0;
vec4 u_xlat1;
vec4 u_xlat2;
float u_xlat3;
bool u_xlatb4;
float u_xlat9;
void main()
{
    //ADD
    u_xlat0.xyz = (-vs_LocalPosition0.xyz) + cameraLocalPos.xyz;
    //DP3
    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
    //RSQ
    u_xlat9 = inversesqrt(u_xlat9);
    //MAD
    u_xlat0.xyz = u_xlat0.xyz * vec3(u_xlat9) + (-directionalLights[0].direction.xyz);
    //DP3
    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
    //RSQ
    u_xlat9 = inversesqrt(u_xlat9);
    //MUL
    u_xlat0.xyz = vec3(u_xlat9) * u_xlat0.xyz;
    //DP3
    u_xlat0.x = dot(vs_Normal0.xyz, u_xlat0.xyz);
    //LOG
    u_xlat3 = log2(abs(u_xlat0.x));
    //LT
    u_xlatb0 = 0.0<u_xlat0.x;
    //MUL
    u_xlat3 = u_xlat3 * 255.0;
    //EXP
    u_xlat3 = exp2(u_xlat3);
    //MOV
    u_xlat1.x = directionalLights[0].base.color.x;
    //MOV
    u_xlat1.y = float(0.0);
    //MOV
    u_xlat1.z = float(0.0);
    //MOV
    u_xlat1.w = float(1.0);
    //MUL
    u_xlat1.xw = vec2(u_xlat3) * u_xlat1.xw;
    //AND
    u_xlati0 = ivec4((uvec4(bvec4(u_xlatb0)) * 0xFFFFFFFFu) & floatBitsToUint(u_xlat1));
    //DP3
    u_xlat1.x = dot(vs_Normal0.xyz, vs_Normal0.xyz);
    //RSQ
    u_xlat1.x = inversesqrt(u_xlat1.x);
    //MUL
    u_xlat1.xyz = u_xlat1.xxx * vs_Normal0.xyz;
    //DP3
    u_xlat1.x = dot(u_xlat1.xyz, (-directionalLights[0].direction.xyz));
    //LT
    u_xlatb4 = 0.0<u_xlat1.x;
    //AND
    u_xlat0 = bool(u_xlatb4) ? intBitsToFloat(u_xlati0) : vec4(0.0, 0.0, 0.0, 0.0);
    //MOV
    u_xlat2.xyz = directionalLights[0].base.color.xyz;
    //MOV
    u_xlat2.w = 1.0;
    //MUL
    u_xlat2 = u_xlat2 * vec4(vec4(directionalLights[0].base.intensity, directionalLights[0].base.intensity, directionalLights[0].base.intensity, directionalLights[0].base.intensity));
    //MUL
    u_xlat2 = u_xlat1.xxxx * u_xlat2;
    //AND
    u_xlat1 = bool(u_xlatb4) ? u_xlat2 : vec4(0.0, 0.0, 0.0, 0.0);
    //ADD
    u_xlat2.w = u_xlat0.w + u_xlat1.w;
    //ADD
    u_xlat2.xyz = u_xlat1.xyz + skyLight.base.color.xyz;
    //MOV
    u_xlat0.w = 1.0;
    //ADD
    u_xlat0 = u_xlat0 + u_xlat2;
    u_xlat0 = clamp(u_xlat0, 0.0, 1.0);
    //SAMPLE
    u_xlat1 = texture(tex, vs_TexCoord0.xy);
    //MUL
    SV_Target0 = u_xlat0 * u_xlat1;
    //RET
    return;
}
