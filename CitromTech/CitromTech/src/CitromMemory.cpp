#include "CitromMemory.h"

#include <memory>

namespace Citrom::Memory
{
	void* Allocate(size_t size)
	{
		return malloc(size);
	}
	void* AllocateZero(size_t size)
	{
		void* memoryBlock = Allocate(size);
		Zero(memoryBlock, size);
		return memoryBlock;
	}
	void* ContiguousAllocate(size_t elementCount, size_t elementSize)
	{
		return calloc(elementCount, elementSize);
	}
	void* AllocateAddMemory(void* ptr, size_t currentSize, size_t sizeToAdd)
	{
		return Reallocate(ptr, currentSize + sizeToAdd);
	}
	void* Reallocate(void* ptr, size_t newSize)
	{
		return realloc(ptr, newSize);
	}
	void Free(void* ptr, size_t size)
	{
		free(ptr);
	}
	void* Copy(void* destination, const void* source, size_t bytesToCopy)
	{
		return memcpy(destination, source, bytesToCopy);
	}
	void* Set(void* ptr, const int valueForEachByte, size_t bytesToSet)
	{
		return memset(ptr, valueForEachByte, bytesToSet);
	}
	void Zero(void* ptr, size_t memoryBlockSize)
	{
		Set(ptr, 0, memoryBlockSize);
	}
	void* StackAllocate(size_t size)
	{
		//return _malloca(size); // generally safer but requires the use of _freea, because it allocates memory on the heap. It is meant for allocating memory that is automatically freed when it goes out of scope.
		//return _alloca(size);
		return alloca(size);
	}
}

// TODO: fix this
#ifdef CT_DEBUG
struct AllocationMetrics
{
	uint32 totalAllocated = 0;
	uint32 totalFreed = 0;

	uint32 GetCurrentUsage() { return totalAllocated - totalFreed; }
};

static AllocationMetrics sg_AllocationMetrics;

#include "Logger/Logger.h"
void* operator new(size_t size)
{
	sg_AllocationMetrics.totalAllocated += size;
	//CT_TRACE("[new]: Allocating {} bytes; Current Usage: {}", (unsigned int)size, (unsigned int)sg_AllocationMetrics.GetCurrentUsage());

	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	sg_AllocationMetrics.totalFreed += size;
	//CT_TRACE("[delete]: Freeing {} bytes; Current Usage: {}", (unsigned int)size, (unsigned int)sg_AllocationMetrics.GetCurrentUsage());

	//if (memory)
	free(memory);
}
#endif