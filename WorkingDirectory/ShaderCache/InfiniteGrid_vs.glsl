#version 330

const int _16[6] = int[](0, 2, 1, 2, 0, 3);
const vec3 _37[4] = vec3[](vec3(-1.0, 0.0, -1.0), vec3(1.0, 0.0, -1.0), vec3(1.0, 0.0, 1.0), vec3(-1.0, 0.0, 1.0));

layout(std140) uniform UBO
{
    mat4 VP;
    float GridSize;
    vec3 CameraWorldPos;
} _47;

out vec3 WorldPos;

void main()
{
    int Index = _16[gl_VertexID];
    vec3 vPos3 = _37[Index] * _47.GridSize;
    vPos3.x += _47.CameraWorldPos.x;
    vPos3.z += _47.CameraWorldPos.z;
    vec4 vPos4 = vec4(vPos3, 1.0);
    gl_Position = _47.VP * vPos4;
    WorldPos = vPos3;
}

