static const int _16[6] = { 0, 2, 1, 2, 0, 3 };
static const float3 _37[4] = { float3(-1.0f, 0.0f, -1.0f), float3(1.0f, 0.0f, -1.0f), float3(1.0f, 0.0f, 1.0f), float3(-1.0f, 0.0f, 1.0f) };

cbuffer UBO : register(b0)
{
    row_major float4x4 _47_VP : packoffset(c0);
    float _47_GridSize : packoffset(c4);
    float3 _47_CameraWorldPos : packoffset(c5);
};


static float4 gl_Position;
static int gl_VertexIndex;
static float3 WorldPos;

struct SPIRV_Cross_Input
{
    uint gl_VertexIndex : SV_VertexID;
};

struct SPIRV_Cross_Output
{
    float3 WorldPos : TEXCOORD1;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    int Index = _16[gl_VertexIndex];
    float3 vPos3 = _37[Index] * _47_GridSize;
    vPos3.x += _47_CameraWorldPos.x;
    vPos3.z += _47_CameraWorldPos.z;
    float4 vPos4 = float4(vPos3, 1.0f);
    gl_Position = mul(vPos4, _47_VP);
    WorldPos = vPos3;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    gl_VertexIndex = int(stage_input.gl_VertexIndex);
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.WorldPos = WorldPos;
    return stage_output;
}
