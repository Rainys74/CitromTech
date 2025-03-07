// Citrom Tech Shader Interoperability
#ifndef CT_SHADERINTEROP_CORE_H
#define CT_SHADERINTEROP_CORE_H

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
#define CTSI_NAMESPACE_BEGIN namespace Citrom { namespace CTSI_NAMESPACE {
#define CTSI_NAMESPACE_END } }

//#define CTSI_NAMESPACE(x) namespace x
#else
#define CTSI_NAMESPACE_BEGIN
#define CTSI_NAMESPACE_END
#endif

//CTSI_NAMESPACE_BEGIN(CTSI_NAMESPACE)

// Types
/*#ifdef CTSI_LANGUAGE_CPP
#include "Math/Vector.h"
using float2 = Citrom::Math::Vector2; // TODO: maybe switch to a specific type to not cause any name clashes? like SIVectorX or something like that?
using float3 = Citrom::Math::Vector3;
using float4 = Citrom::Math::Vector4;

#include "Math/Matrix4x4.h"
using matrix = Citrom::Math::Matrix4x4;
#elif defined(CTSI_LANGUAGE_HLSL)
//#define alignas()
#endif*/

#ifdef CTSI_LANGUAGE_CPP

#include "Math/Vector.h"
#define CTSI_TYPE_FLOAT2 Citrom::Math::Vector2
#define CTSI_TYPE_FLOAT3 Citrom::Math::Vector3
#define CTSI_TYPE_FLOAT4 Citrom::Math::Vector4

#include "Math/Color.h"
#define CTSI_TYPE_COLOR3 Citrom::Math::ColorF32x3
#define CTSI_TYPE_COLOR4 Citrom::Math::ColorF32x4

#include "Math/Matrix4x4.h"
#define CTSI_TYPE_MATRIX Citrom::Math::Matrix4x4

#include "Core.h"
#define CTSI_TYPE_UINT uint32

#elif defined(CTSI_LANGUAGE_HLSL)
#define CTSI_TYPE_FLOAT2 float2
#define CTSI_TYPE_FLOAT3 float3
#define CTSI_TYPE_FLOAT4 float4

#define CTSI_TYPE_COLOR3 CTSI_TYPE_FLOAT3
#define CTSI_TYPE_COLOR4 CTSI_TYPE_FLOAT4

#define CTSI_TYPE_MATRIX matrix

#define CTSI_TYPE_UINT uint

#define alignas(x)
#endif

#ifdef CTSI_LANGUAGE_CPP
#define CTSI_CONSTANT_BUFFER(NAME, SLOT) struct alignas(16) NAME
#elif defined(CTSI_LANGUAGE_HLSL)
#define CTSI_CONSTANT_BUFFER(NAME, SLOT) cbuffer NAME : register(b ## SLOT)
#endif

//CTSI_NAMESPACE_END()
#endif // CT_SHADERINTEROP_CORE_H