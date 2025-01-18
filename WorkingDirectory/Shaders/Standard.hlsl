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
};

float4 vsmain(VSInput input) : SV_Position
{
    return float4(input.pos, 1.0f);
}

float4 psmain() : SV_Target
{
    return float4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
}