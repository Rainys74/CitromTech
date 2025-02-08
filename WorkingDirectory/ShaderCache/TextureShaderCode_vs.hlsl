cbuffer matrices : register(b0)
{
    row_major float4x4 _19_u_MVP : packoffset(c0);
};


static float4 gl_Position;
static float4 position;
static float2 v_TexCoord;
static float2 texCoord;

struct SPIRV_Cross_Input
{
    float4 position : POSITION;
    float2 texCoord : NORMAL;
};

struct SPIRV_Cross_Output
{
    float2 v_TexCoord : TEXCOORD1;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = mul(position, _19_u_MVP);
    v_TexCoord = texCoord;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    position = stage_input.position;
    texCoord = stage_input.texCoord;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.v_TexCoord = v_TexCoord;
    return stage_output;
}
