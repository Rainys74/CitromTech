#version 450 core

// GLSL Compute shaders do not work as expected currently

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout(std430, binding = 0) buffer DataBuffer {
    uint data[];
};

void main() {
    uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * 16;
    data[index] += 1; // Simple operation to modify buffer data
}