#pragma once

#include "Core.h"

namespace Citrom::Memory
{
	void* Allocate(size_t size);
	void* AllocateZero(size_t size);
	void* ContiguousAllocate(size_t elementCount, size_t elementSize);

	void* AllocateAddMemory(void* ptr, size_t currentSize, size_t sizeToAdd);
	void* Reallocate(void* ptr, size_t newSize);

	void Free(void* ptr);

	void* Copy(void* destination, const void* source, size_t bytesToCopy);
	void* Set(void* ptr, const int valueForEachByte, size_t bytesToSet);

	void Zero(void* ptr, size_t memoryBlockSize);

	void* StackAllocate(size_t size);
}