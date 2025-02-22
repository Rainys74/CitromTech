#version 330

layout(std140) uniform UBO
{
    vec3 CameraWorldPos;
    float GridSize;
    float GridMinPixelsBetweenCells;
    float GridCellSize;
    vec4 GridColorThin;
    vec4 GridColorThick;
} _92;

in vec3 WorldPos;
layout(location = 0) out vec4 FragColor;

float gllog10(float x)
{
    return log(x) / 2.302585124969482421875;
}

vec2 satv(vec2 x)
{
    return clamp(x, vec2(0.0), vec2(1.0));
}

float max2(vec2 v)
{
    return max(v.x, v.y);
}

float satf(float x)
{
    return clamp(x, 0.0, 1.0);
}

void main()
{
    vec2 dvx = vec2(dFdx(WorldPos.x), dFdy(WorldPos.x));
    vec2 dvy = vec2(dFdx(WorldPos.z), dFdy(WorldPos.z));
    float lx = length(dvx);
    float ly = length(dvy);
    vec2 dudv = vec2(lx, ly);
    float l = length(dudv);
    float param = (l * _92.GridMinPixelsBetweenCells) / _92.GridCellSize;
    float LOD = max(0.0, gllog10(param) + 1.0);
    float GridCellSizeLod0 = _92.GridCellSize * pow(10.0, floor(LOD));
    float GridCellSizeLod1 = GridCellSizeLod0 * 10.0;
    float GridCellSizeLod2 = GridCellSizeLod1 * 10.0;
    dudv *= 4.0;
    vec2 mod_div_dudv = mod(WorldPos.xz, vec2(GridCellSizeLod0)) / dudv;
    vec2 param_1 = mod_div_dudv;
    vec2 param_2 = vec2(1.0) - abs((satv(param_1) * 2.0) - vec2(1.0));
    float Lod0a = max2(param_2);
    mod_div_dudv = mod(WorldPos.xz, vec2(GridCellSizeLod1)) / dudv;
    vec2 param_3 = mod_div_dudv;
    vec2 param_4 = vec2(1.0) - abs((satv(param_3) * 2.0) - vec2(1.0));
    float Lod1a = max2(param_4);
    mod_div_dudv = mod(WorldPos.xz, vec2(GridCellSizeLod2)) / dudv;
    vec2 param_5 = mod_div_dudv;
    vec2 param_6 = vec2(1.0) - abs((satv(param_5) * 2.0) - vec2(1.0));
    float Lod2a = max2(param_6);
    float LOD_fade = fract(LOD);
    vec4 Color;
    if (Lod2a > 0.0)
    {
        Color = _92.GridColorThick;
        Color.w *= Lod2a;
    }
    else
    {
        if (Lod1a > 0.0)
        {
            Color = mix(_92.GridColorThick, _92.GridColorThin, vec4(LOD_fade));
            Color.w *= Lod1a;
        }
        else
        {
            Color = _92.GridColorThin;
            Color.w *= (Lod0a * (1.0 - LOD_fade));
        }
    }
    float param_7 = length(WorldPos.xz - _92.CameraWorldPos.xz) / _92.GridSize;
    float OpacityFalloff = 1.0 - satf(param_7);
    Color.w *= OpacityFalloff;
    FragColor = Color;
}

