// Full-Screen Triangle
static const float2 positions[3] =
{
    float2(-1.0, 1.0),  // Top-left
    float2(3.0, 1.0),   // Top-right beyond screen (to complete the tri)
    float2(-1.0, -3.0)  // Bottom-left beyond screen
};

static const float2 uvs[3] =
{
    float2(0.0, 0.0),
    float2(2.0, 0.0),
    float2(0.0, 2.0)
};
// also do i need this?
/*
static const float2 uvs[3] = 
{
    float2(0.0, 1.0),  // Top-left
    float2(2.0, 1.0),  // Top-right
    float2(0.0, -1.0)  // Bottom-left
};
*/

struct VSInput
{
    uint vertexID : SV_VertexID;
};
struct VSOutput
{
    float2 uv : TexCoord;
    float4 position : SV_Position;
};

VSOutput vsmain(VSInput input)
{
    VSOutput vso;
    vso.position = float4(positions[input.vertexID], 0.0, 1.0);
    vso.uv = uvs[input.vertexID];
    return vso;
}

float4 psmain(VSOutput input) : SV_Target
{
    // do i need this? input.uv = input.uv * 0.5 + 0.5;
    return float4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
}