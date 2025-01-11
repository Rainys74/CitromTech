#version 330

layout(std140) uniform matrices
{
    mat4 u_MVP;
} _19;

layout(location = 0) in vec4 position;
out vec2 v_TexCoord;
layout(location = 1) in vec2 texCoord;

void main()
{
    gl_Position = _19.u_MVP * position;
    v_TexCoord = texCoord;
}

