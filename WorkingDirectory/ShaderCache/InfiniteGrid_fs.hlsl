cbuffer UBO : register(b1)
{
    float3 _92_CameraWorldPos : packoffset(c0);
    float _92_GridSize : packoffset(c0.w);
    float _92_GridMinPixelsBetweenCells : packoffset(c1);
    float _92_GridCellSize : packoffset(c1.y);
    float4 _92_GridColorThin : packoffset(c2);
    float4 _92_GridColorThick : packoffset(c3);
};


static float3 WorldPos;
static float4 FragColor;

struct SPIRV_Cross_Input
{
    float3 WorldPos : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float4 FragColor : SV_Target0;
};

float mod(float x, float y)
{
    return x - y * floor(x / y);
}

float2 mod(float2 x, float2 y)
{
    return x - y * floor(x / y);
}

float3 mod(float3 x, float3 y)
{
    return x - y * floor(x / y);
}

float4 mod(float4 x, float4 y)
{
    return x - y * floor(x / y);
}

float gllog10(float x)
{
    return log(x) / 2.302585124969482421875f;
}

float2 satv(float2 x)
{
    return clamp(x, 0.0f.xx, 1.0f.xx);
}

float max2(float2 v)
{
    return max(v.x, v.y);
}

float satf(float x)
{
    return clamp(x, 0.0f, 1.0f);
}

void frag_main()
{
    float2 dvx = float2(ddx(WorldPos.x), ddy(WorldPos.x));
    float2 dvy = float2(ddx(WorldPos.z), ddy(WorldPos.z));
    float lx = length(dvx);
    float ly = length(dvy);
    float2 dudv = float2(lx, ly);
    float l = length(dudv);
    float param = (l * _92_GridMinPixelsBetweenCells) / _92_GridCellSize;
    float LOD = max(0.0f, gllog10(param) + 1.0f);
    float GridCellSizeLod0 = _92_GridCellSize * pow(10.0f, floor(LOD));
    float GridCellSizeLod1 = GridCellSizeLod0 * 10.0f;
    float GridCellSizeLod2 = GridCellSizeLod1 * 10.0f;
    dudv *= 4.0f;
    float2 mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod0.xx) / dudv;
    float2 param_1 = mod_div_dudv;
    float2 param_2 = 1.0f.xx - abs((satv(param_1) * 2.0f) - 1.0f.xx);
    float Lod0a = max2(param_2);
    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod1.xx) / dudv;
    float2 param_3 = mod_div_dudv;
    float2 param_4 = 1.0f.xx - abs((satv(param_3) * 2.0f) - 1.0f.xx);
    float Lod1a = max2(param_4);
    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod2.xx) / dudv;
    float2 param_5 = mod_div_dudv;
    float2 param_6 = 1.0f.xx - abs((satv(param_5) * 2.0f) - 1.0f.xx);
    float Lod2a = max2(param_6);
    float LOD_fade = frac(LOD);
    float4 Color;
    if (Lod2a > 0.0f)
    {
        Color = _92_GridColorThick;
        Color.w *= Lod2a;
    }
    else
    {
        if (Lod1a > 0.0f)
        {
            Color = lerp(_92_GridColorThick, _92_GridColorThin, LOD_fade.xxxx);
            Color.w *= Lod1a;
        }
        else
        {
            Color = _92_GridColorThin;
            Color.w *= (Lod0a * (1.0f - LOD_fade));
        }
    }
    float param_7 = length(WorldPos.xz - _92_CameraWorldPos.xz) / _92_GridSize;
    float OpacityFalloff = 1.0f - satf(param_7);
    Color.w *= OpacityFalloff;
    FragColor = Color;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    WorldPos = stage_input.WorldPos;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output.FragColor = FragColor;
    return stage_output;
}
