#pragma once

#include "Core.h" // Citrom Tech Core

#include "CTLConfig.h" // Custom Config for defines

//CTL_MEMORY_NEW
//CTL_MEMORY_NEWARR
//CTL_MEMORY_DELETE
//CTL_MEMORY_DELETEARR

// Use standard C memory functions if custom ones weren't defined in CTLConfig.h
#if !defined(CTL_MEMORY_COPY) && !defined(CTL_MEMORY_REALLOCATE) && !defined(CTL_MEMORY_ALLOCATE_ADD_MEMORY)
#include <memory>
#define CTL_MEMORY_REALLOCATE(PTR, NEWSIZE) realloc(PTR, NEWSIZE)
#define CTL_MEMORY_COPY(DESTINATION, SOURCE, BYTESTOCOPY) memcpy(DESTINATION, SOURCE, BYTESTOCOPY)
#define CTL_MEMORY_ALLOCATE_ADD_MEMORY(PTR, CURRENTSIZE, SIZETOADD) CTL_MEMORY_REALLOCATE(PTR, (CURRENTSIZE) + (SIZETOADD))
#else
    // Throw errors if all of them weren't defined
    #if !defined(CTL_MEMORY_COPY) || !defined(CTL_MEMORY_REALLOCATE) || !defined(CTL_MEMORY_ALLOCATE_ADD_MEMORY)
    #error "All memory macros (e.g. CTL_MEMORY_COPY and CTL_MEMORY_REALLOCATE) must be defined, or none at all, in CTLConfig.h. Please define them to ensure proper memory management."
    #endif
#endif