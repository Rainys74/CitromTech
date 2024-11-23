#pragma once

// Comment this out if you figured out a way
// how to build this as a static library
#define GLSLCC_BUILD_AS_DLL

#ifdef GLSLCC_BUILD_AS_DLL
#if defined(_MSC_VER) && defined(_WIN32)
#define GLSLCC_EXPORT __declspec(dllexport)
#else
#define GLSLCC_EXPORT __attribute__((visibility("default"))) // __attribute__((dllexport))
#endif
#else
#define GLSLCC_EXPORT
#endif

GLSLCC_EXPORT int glslcc_exec(int argc, char* argv[]);