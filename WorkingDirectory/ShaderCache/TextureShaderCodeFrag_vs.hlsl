Texture2D<float4> u_Texture : register(t0);
SamplerState _u_Texture_sampler : register(s0);

static float2 v_TexCoord;
static float4 color;

struct SPIRV_Cross_Input
{
    float2 v_TexCoord : NORMAL;
};

struct SPIRV_Cross_Output
{
    float4 color : TEXCOORD0;
};

void vert_main()
{
    float4 texColor = u_Texture.SampleLevel(_u_Texture_sampler, v_TexCoord, 0.0f);
    color = texColor;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    v_TexCoord = stage_input.v_TexCoord;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.color = color;
    return stage_output;
}
