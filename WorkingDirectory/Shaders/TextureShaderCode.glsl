//@begin_vert
#version 460
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

layout (location = 1) out vec2 v_TexCoord;

//layout (binding = 0) uniform mat4 u_MVP;
layout (binding=0, std140) uniform matrices {
        mat4 u_MVP;
};

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};
//@end

//@begin_frag
#version 460

//precision mediump float;

layout (location = 0) out vec4 color;

layout (location = 1) in vec2 v_TexCoord;

layout (binding = 0) uniform sampler2D sampler_u_Texture_sampler;

void main()
{
	vec4 texColor = texture(sampler_u_Texture_sampler, v_TexCoord);
	color = texColor;
};
//@end
