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
float ImmCB_0_0_0[6];
vec3 ImmCB_0_0_1[6];
layout(location = 0) uniform 	mat4x4 _47_VP;
uniform 	float _47_GridSize;
uniform 	vec3 _47_CameraWorldPos;
layout(location = 0) out vec3 vs_TEXCOORD1;
vec4 u_xlat0;
int u_xlati0;
vec4 u_xlat1;
void main()
{
    //--- Start Early Main ---
	ImmCB_0_0_0[0] = 0.0;
	ImmCB_0_0_0[1] = 2.80259693e-45;
	ImmCB_0_0_0[2] = 1.40129846e-45;
	ImmCB_0_0_0[3] = 2.80259693e-45;
	ImmCB_0_0_0[4] = 0.0;
	ImmCB_0_0_0[5] = 4.20389539e-45;
	ImmCB_0_0_1[0] = vec3(-1.0, 0.0, -1.0);
	ImmCB_0_0_1[1] = vec3(1.0, 0.0, -1.0);
	ImmCB_0_0_1[2] = vec3(1.0, 0.0, 1.0);
	ImmCB_0_0_1[3] = vec3(-1.0, 0.0, 1.0);
	ImmCB_0_0_1[4] = vec3(0.0, 0.0, 0.0);
	ImmCB_0_0_1[5] = vec3(0.0, 0.0, 0.0);
    //--- End Early Main ---
    //MOV
    u_xlati0 = gl_VertexID;
    //MOV
    u_xlat0.x = ImmCB_0_0_0[u_xlati0];
    //MOV
    u_xlat0.xy = ImmCB_0_0_1[floatBitsToInt(u_xlat0.x)].xz;
    //MAD
    u_xlat0.xy = u_xlat0.xy * vec2(UBOVS._47_GridSize) + UBOVS._47_CameraWorldPos.xz;
    //MOV
    vs_TEXCOORD1.xz = u_xlat0.xy;
    //MOV
    vs_TEXCOORD1.y = 0.0;
    //MUL
    u_xlat1 = u_xlat0.yyyy * UBOVS._47_VP[2];
    //MAD
    u_xlat0 = u_xlat0.xxxx * UBOVS._47_VP[0] + u_xlat1;
    //ADD
    gl_Position = u_xlat0 + UBOVS._47_VP[3];
    //RET
    return;
}
