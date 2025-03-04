Texture2D<float4> u_Texture : register(t0);
SamplerState _u_Texture_sampler : register(s0);

static float2 v_TexCoord;
static float4 color;

struct SPIRV_Cross_Input
{
    float2 v_TexCoord : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float4 color : SV_Target0;
};

void frag_main()
{
    float4 texColor = u_Texture.Sample(_u_Texture_sampler, v_TexCoord);
    color = texColor;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    v_TexCoord = stage_input.v_TexCoord;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output.color = color;
    return stage_output;
}
