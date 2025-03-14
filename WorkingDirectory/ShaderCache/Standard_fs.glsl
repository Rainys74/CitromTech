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
UNITY_BINDING(2) uniform StandardMaterial {
	vec3 mat_Albedo;
	float mat_Metallic;
	float mat_Roughness;
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
vec4 u_xlat3;
float u_xlat4;
bool u_xlatb5;
float u_xlat8;
float u_xlat9;
float u_xlat12;
void main()
{
    //ADD
    u_xlat0.xyz = (-vs_LocalPosition0.xyz) + cameraLocalPos.xyz;
    //DP3
    u_xlat12 = dot(u_xlat0.xyz, u_xlat0.xyz);
    //RSQ
    u_xlat12 = inversesqrt(u_xlat12);
    //MAD
    u_xlat0.xyz = u_xlat0.xyz * vec3(u_xlat12) + (-directionalLights[0].direction.xyz);
    //DP3
    u_xlat12 = dot(u_xlat0.xyz, u_xlat0.xyz);
    //RSQ
    u_xlat12 = inversesqrt(u_xlat12);
    //MUL
    u_xlat0.xyz = vec3(u_xlat12) * u_xlat0.xyz;
    //DP3
    u_xlat0.x = dot(vs_Normal0.xyz, u_xlat0.xyz);
    //LOG
    u_xlat4 = log2(abs(u_xlat0.x));
    //LT
    u_xlatb0 = 0.0<u_xlat0.x;
    //ADD
    u_xlat8 = (-mat_Roughness) + 1.0;
    //MUL
    u_xlat8 = u_xlat8 * 255.0;
    //MUL
    u_xlat4 = u_xlat4 * u_xlat8;
    //EXP
    u_xlat4 = exp2(u_xlat4);
    //ADD
    u_xlat1.xyz = mat_Albedo.xyz + vec3(-0.0399999991, -0.0399999991, -0.0399999991);
    //MAD
    u_xlat1.xyz = vec3(vec3(mat_Metallic, mat_Metallic, mat_Metallic)) * u_xlat1.xyz + vec3(0.0399999991, 0.0399999991, 0.0399999991);
    //MUL
    u_xlat1.xyz = u_xlat1.xyz * directionalLights[0].base.color.xyz;
    //MOV
    u_xlat1.w = 1.0;
    //MUL
    u_xlat1 = vec4(u_xlat4) * u_xlat1;
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
    u_xlatb5 = 0.0<u_xlat1.x;
    //AND
    u_xlat0 = bool(u_xlatb5) ? intBitsToFloat(u_xlati0) : vec4(0.0, 0.0, 0.0, 0.0);
    //ADD
    u_xlat9 = (-mat_Metallic) + 1.0;
    //MUL
    u_xlat2.xyz = vec3(u_xlat9) * mat_Albedo.xyz;
    //MUL
    u_xlat3.xyz = vec3(vec3(directionalLights[0].base.intensity, directionalLights[0].base.intensity, directionalLights[0].base.intensity)) * directionalLights[0].base.color.xyz;
    //MOV
    u_xlat3.w = directionalLights[0].base.intensity;
    //MOV
    u_xlat2.w = 1.0;
    //MUL
    u_xlat2 = u_xlat2 * u_xlat3;
    //MUL
    u_xlat2 = u_xlat1.xxxx * u_xlat2;
    //AND
    u_xlat1 = bool(u_xlatb5) ? u_xlat2 : vec4(0.0, 0.0, 0.0, 0.0);
    //ADD
    u_xlat2.w = u_xlat0.w + u_xlat1.w;
    //ADD
    u_xlat2.xyz = u_xlat1.xyz + skyLight.base.color.xyz;
    //MOV
    u_xlat0.w = 1.0;
    //ADD
    u_xlat0 = u_xlat0 + u_xlat2;
    //SAMPLE
    u_xlat1 = texture(tex, vs_TexCoord0.xy);
    //MUL
    SV_Target0 = u_xlat0 * u_xlat1;
    //RET
    return;
}
