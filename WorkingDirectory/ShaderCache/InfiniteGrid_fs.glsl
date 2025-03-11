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
UNITY_BINDING(0) uniform UBO {
	vec3 _92_CameraWorldPos;
	float _92_GridSize;
	float _92_GridMinPixelsBetweenCells;
	float _92_GridCellSize;
	vec4 _92_GridColorThin;
	vec4 _92_GridColorThick;
};
layout(location = 0) in  vec3 vs_TEXCOORD1;
layout(location = 0) out vec4 SV_Target0;
vec4 u_xlat0;
bvec3 u_xlatb0;
vec4 u_xlat1;
vec4 u_xlat2;
vec4 u_xlat3;
vec4 u_xlat4;
vec2 u_xlat5;
vec2 u_xlat10;
void main()
{
    //DERIV_RTX
    u_xlat0.xy = dFdx(vs_TEXCOORD1.xz);
    //DERIV_RTY
    u_xlat0.zw = dFdy(vs_TEXCOORD1.xz);
    //DP2
    u_xlat0.x = dot(u_xlat0.xz, u_xlat0.xz);
    //DP2
    u_xlat0.y = dot(u_xlat0.yw, u_xlat0.yw);
    //SQRT
    u_xlat1 = sqrt(u_xlat0.xyxy);
    //DP2
    u_xlat0.x = dot(u_xlat1.zw, u_xlat1.zw);
    //MUL
    u_xlat1 = u_xlat1 * vec4(4.0, 4.0, 4.0, 4.0);
    //SQRT
    u_xlat0.x = sqrt(u_xlat0.x);
    //MUL
    u_xlat0.x = u_xlat0.x * _92_GridMinPixelsBetweenCells;
    //DIV
    u_xlat0.x = u_xlat0.x / _92_GridCellSize;
    //LOG
    u_xlat0.x = log2(u_xlat0.x);
    //MAD
    u_xlat0.x = u_xlat0.x * 0.30102998 + 1.0;
    //MAX
    u_xlat0.x = max(u_xlat0.x, 0.0);
    //ROUND_NI
    u_xlat5.x = floor(u_xlat0.x);
    //FRC
    u_xlat0.x = fract(u_xlat0.x);
    //MUL
    u_xlat5.x = u_xlat5.x * 3.32192802;
    //EXP
    u_xlat5.x = exp2(u_xlat5.x);
    //MUL
    u_xlat5.x = u_xlat5.x * _92_GridCellSize;
    //DIV
    u_xlat10.xy = vs_TEXCOORD1.xz / u_xlat5.xx;
    //ROUND_NI
    u_xlat10.xy = floor(u_xlat10.xy);
    //MAD
    u_xlat10.xy = (-u_xlat5.xx) * u_xlat10.xy + vs_TEXCOORD1.xz;
    //MUL
    u_xlat2 = u_xlat5.xxxx * vec4(10.0, 10.0, 100.0, 100.0);
    //DIV
    u_xlat5.xy = u_xlat10.xy / u_xlat1.xy;
    u_xlat5.xy = clamp(u_xlat5.xy, 0.0, 1.0);
    //MAD
    u_xlat5.xy = u_xlat5.xy * vec2(2.0, 2.0) + vec2(-1.0, -1.0);
    //ADD
    u_xlat5.xy = -abs(u_xlat5.xy) + vec2(1.0, 1.0);
    //MAX
    u_xlat5.x = max(u_xlat5.y, u_xlat5.x);
    //ADD
    u_xlat10.x = (-u_xlat0.x) + 1.0;
    //MUL
    u_xlat5.x = u_xlat10.x * u_xlat5.x;
    //MUL
    u_xlat3.w = u_xlat5.x * _92_GridColorThin.w;
    //DIV
    u_xlat4 = vs_TEXCOORD1.xzxz / u_xlat2.yyww;
    //ROUND_NI
    u_xlat4 = floor(u_xlat4);
    //MAD
    u_xlat2 = (-u_xlat2) * u_xlat4 + vs_TEXCOORD1.xzxz;
    //DIV
    u_xlat1 = u_xlat2 / u_xlat1;
    u_xlat1 = clamp(u_xlat1, 0.0, 1.0);
    //MAD
    u_xlat1 = u_xlat1 * vec4(2.0, 2.0, 2.0, 2.0) + vec4(-1.0, -1.0, -1.0, -1.0);
    //ADD
    u_xlat1 = -abs(u_xlat1) + vec4(1.0, 1.0, 1.0, 1.0);
    //MAX
    u_xlat5.xy = max(u_xlat1.wy, u_xlat1.zx);
    //ADD
    u_xlat1 = _92_GridColorThin + (-_92_GridColorThick);
    //MAD
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + _92_GridColorThick;
    //MUL
    u_xlat1.w = u_xlat5.y * u_xlat1.w;
    //LT
    u_xlatb0.xz = lessThan(vec4(0.0, 0.0, 0.0, 0.0), u_xlat5.xxyx).xz;
    //MUL
    u_xlat2.w = u_xlat5.x * _92_GridColorThick.w;
    //MOV
    u_xlat3.xyz = _92_GridColorThin.xyz;
    //MOVC
    u_xlat1 = (u_xlatb0.z) ? u_xlat1 : u_xlat3;
    //MOV
    u_xlat2.xyz = _92_GridColorThick.xyz;
    //MOVC
    u_xlat0 = (u_xlatb0.x) ? u_xlat2 : u_xlat1;
    //ADD
    u_xlat1.xy = vs_TEXCOORD1.xz + (-_92_CameraWorldPos.xz);
    //DP2
    u_xlat1.x = dot(u_xlat1.xy, u_xlat1.xy);
    //SQRT
    u_xlat1.x = sqrt(u_xlat1.x);
    //DIV
    u_xlat1.x = u_xlat1.x / _92_GridSize;
    u_xlat1.x = clamp(u_xlat1.x, 0.0, 1.0);
    //ADD
    u_xlat1.x = (-u_xlat1.x) + 1.0;
    //MUL
    SV_Target0.w = u_xlat0.w * u_xlat1.x;
    //MOV
    SV_Target0.xyz = u_xlat0.xyz;
    //RET
    return;
}
