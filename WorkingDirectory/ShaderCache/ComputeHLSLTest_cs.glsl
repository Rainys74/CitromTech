#version 440

#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
#if HLSLCC_ENABLE_UNIFORM_BUFFERS
#define UNITY_UNIFORM
#else
#define UNITY_UNIFORM uniform
#endif
#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
#if UNITY_SUPPORTS_UNIFORM_LOCATION
#define UNITY_LOCATION(x) layout(location = x)
#define UNITY_BINDING(x) layout(binding = x, std140)
#else
#define UNITY_LOCATION(x)
#define UNITY_BINDING(x) layout(std140)
#endif
precise vec4 u_xlat_precise_vec4;
precise ivec4 u_xlat_precise_ivec4;
precise bvec4 u_xlat_precise_bvec4;
precise uvec4 u_xlat_precise_uvec4;
 struct OutputBuffer_type {
	uint[1] value;
};

layout(std430, binding = 0) buffer OutputBuffer {
	OutputBuffer_type OutputBuffer_buf[];
};
int u_xlati0;
int u_xlati1;
layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    u_xlati0 = int(gl_GlobalInvocationID.y) << 4;
    u_xlati0 = u_xlati0 + int(gl_GlobalInvocationID.x);
    u_xlati1 = int(OutputBuffer_buf[u_xlati0].value[(0 >> 2) + 0]);
    u_xlati1 = u_xlati1 + 1;
    OutputBuffer_buf[u_xlati0].value[(0 >> 2)] = uint(u_xlati1);
    return;
}
