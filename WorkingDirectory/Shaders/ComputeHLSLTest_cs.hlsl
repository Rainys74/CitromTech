RWStructuredBuffer<uint> OutputBuffer : register(u0);

// Define the compute shader thread group size
[numthreads(16, 16, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
    // Compute a unique 1D index from the 2D thread ID
    uint index = threadID.x + (threadID.y * 16); // Assuming 16x16 dispatch

    // Simple test: Increment each element
    OutputBuffer[index] += 1;
}