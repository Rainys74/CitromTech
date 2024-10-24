#pragma once

#include "CitromMemory.h"
#define CTL_MEMORY_REALLOCATE(PTR, NEWSIZE) Citrom::Memory::Reallocate(PTR, NEWSIZE)
#define CTL_MEMORY_COPY(DESTINATION, SOURCE, BYTESTOCOPY) Citrom::Memory::Copy(DESTINATION, SOURCE, BYTESTOCOPY)
