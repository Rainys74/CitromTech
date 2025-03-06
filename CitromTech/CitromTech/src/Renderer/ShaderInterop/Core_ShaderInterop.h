// Citrom Tech Shader Interoperability

// Language detection
#ifdef __cplusplus
#define CTSI_LANGUAGE_CPP
#elif defined(__STDC__)
#error "C is not supported in Citrom Tech!"
#elif defined(INITIAL_SHADER_LANG_HLSL)
#define CTSI_LANGUAGE_HLSL
#elif defined(INITIAL_SHADER_LANG_GLSL)
#error "GLSL Shader Interoperability is currently not supported by Citrom Tech!" // for glsl i'm pretty sure you could use something like #define float4 vec4 to support interoperability
#endif

// Namespace Stuff
#define CTSI_NAMESPACE ShaderInterop

#ifdef CTSI_LANGUAGE_CPP
#define CTSI_NAMESPACE_BEGIN(x) namespace x { // no need for double namespaces as in Renderer::ShaderInterop
#define CTSI_NAMESPACE_END() }

//#define CTSI_NAMESPACE(x) namespace x
#else
#define CTSI_NAMESPACE_BEGIN(x)
#define CTSI_NAMESPACE_END()
#endif

//CTSI_NAMESPACE_BEGIN(CTSI_NAMESPACE)

// Types
#ifdef CTSI_LANGUAGE_CPP
#include "Math/Vector.h"
using float2 = Citrom::Math::Vector2; // TODO: maybe switch to a specific type to not cause any name clashes? like SIVectorX or something like that?
using float3 = Citrom::Math::Vector3;
using float4 = Citrom::Math::Vector4;

#include "Math/Matrix4x4.h"
using matrix = Citrom::Math::Matrix4x4;
#elif defined(CTSI_LANGUAGE_HLSL)
//#define alignas()
#endif

//CTSI_NAMESPACE_END()