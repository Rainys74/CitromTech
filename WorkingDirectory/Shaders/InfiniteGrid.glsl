//@begin_vert

/*

        Copyright 2024 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#version 460

layout(location = 1) out vec3 WorldPos;

layout(binding = 0) uniform UBO {
    mat4 VP;
    float GridSize;
    vec3 CameraWorldPos;
};

const vec3 Pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),
    vec3( 1.0, 0.0, -1.0),
    vec3( 1.0, 0.0,  1.0),
    vec3(-1.0, 0.0,  1.0)
);

const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

void main() 
{
    int Index = Indices[gl_VertexIndex];
    vec3 vPos3 = Pos[Index] * GridSize;

    vPos3.x += CameraWorldPos.x;
    vPos3.z += CameraWorldPos.z;

    vec4 vPos4 = vec4(vPos3, 1.0);

    gl_Position = VP * vPos4;
    WorldPos = vPos3;
}
//@end

//@begin_frag
#version 460

layout(location = 1) in vec3 WorldPos;
layout(location = 0) out vec4 FragColor;

layout(binding = 1) uniform UBO {
    vec3 CameraWorldPos;
    float GridSize;
    float GridMinPixelsBetweenCells;
    float GridCellSize;
    vec4 GridColorThin;
    vec4 GridColorThick;
};

float gllog10(float x) // renamed to not cause ambiguity between MSL's log10
{
    return log(x) / log(10.0);
}

float satf(float x) 
{
    return clamp(x, 0.0, 1.0);
}

vec2 satv(vec2 x) 
{
    return clamp(x, vec2(0.0), vec2(1.0));
}

float max2(vec2 v) 
{
    return max(v.x, v.y);
}

void main() 
{
    vec2 dvx = vec2(dFdx(WorldPos.x), dFdy(WorldPos.x));
    vec2 dvy = vec2(dFdx(WorldPos.z), dFdy(WorldPos.z));

    float lx = length(dvx);
    float ly = length(dvy);
    vec2 dudv = vec2(lx, ly);
    float l = length(dudv);

    float LOD = max(0.0, gllog10(l * GridMinPixelsBetweenCells / GridCellSize) + 1.0);

    float GridCellSizeLod0 = GridCellSize * pow(10.0, floor(LOD));
    float GridCellSizeLod1 = GridCellSizeLod0 * 10.0;
    float GridCellSizeLod2 = GridCellSizeLod1 * 10.0;

    dudv *= 4.0;

    vec2 mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod0) / dudv;
    float Lod0a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod1) / dudv;
    float Lod1a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );
    
    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod2) / dudv;
    float Lod2a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    float LOD_fade = fract(LOD);
    vec4 Color;

    if (Lod2a > 0.0) 
    {
        Color = GridColorThick;
        Color.a *= Lod2a;
    } 
    else 
    {
        if (Lod1a > 0.0) 
        {
            Color = mix(GridColorThick, GridColorThin, LOD_fade);
            Color.a *= Lod1a;
        } 
        else 
        {
            Color = GridColorThin;
            Color.a *= (Lod0a * (1.0 - LOD_fade));
        }
    }
    
    float OpacityFalloff = (1.0 - satf(length(WorldPos.xz - CameraWorldPos.xz) / GridSize));
    Color.a *= OpacityFalloff;

    FragColor = Color;
}
//@end
