/*cbuffer CBuf
{
    matrix transform;
};

float4 vsmain(float3 pos : Position) : SV_Position
{
    //float4 transformedPosition = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    //return float4(transformedPosition.x, transformedPosition.y, 0.0f, 1.0f);
    return float4(pos.x, pos.y, 0.0f, 1.0f);
}

float4 psmain() : SV_Target
{
    return float4(1.0f, 1.0f, 0.0f, 1.0f);
}*/

struct VSInput
{
    float3 pos : Position;
    float3 n : Normal;
    float2 tex : TexCoord1;
};
struct VSOut
{
    float2 tex : TexCoord;
    float4 pos : SV_Position;
};

cbuffer CBuffer1
{
    matrix transform;
};
cbuffer Material : register(b1)
{
    //float u_Test;
    float4 u_ColorData;
};

VSOut vsmain(VSInput input)
{
    VSOut vso;
    vso.pos = mul(float4(input.pos, 1.0f), transform);
    vso.tex = input.tex;
    return vso;
}

Texture2D tex;
SamplerState splr;

float4 psmain(float2 texCoord : TexCoord) : SV_Target
{
    //return float4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    //return tex.Sample(splr, texCoord);
    return u_ColorData;
}