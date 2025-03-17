#version 330

uniform sampler2D sampler_u_Texture_sampler;

in vec2 v_TexCoord;
layout(location = 0) out vec4 color;

void main()
{
    vec4 texColor = texture(sampler_u_Texture_sampler, v_TexCoord);
    color = texColor;
}

