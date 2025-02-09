#pragma once

// Comment this out if you figured out a way
// how to build this as a static library
#define GLSLCC_BUILD_AS_DLL

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#ifdef GLSLCC_BUILD_AS_DLL
#if defined(_MSC_VER) && defined(_WIN32)
#define GLSLCC_EXPORT EXTERN_C __declspec(dllexport)
#else
#define GLSLCC_EXPORT EXTERN_C __attribute__((visibility("default"))) // __attribute__((dllexport))
#endif
#else
#define GLSLCC_EXPORT
#endif

#define GLSLCC_EXIT(ERRORCODE) return ERRORCODE // exit(ERRORCODE)

#define PRINTF(...) printf("[glslcc]: "__VA_ARGS__)

//#define SX_SNPRINTF(...) sx_snprintf(__VA_ARGS__)
#define SX_SNPRINTF sx_snprintf

GLSLCC_EXPORT int glslcc_exec(int argc, char* argv[]);