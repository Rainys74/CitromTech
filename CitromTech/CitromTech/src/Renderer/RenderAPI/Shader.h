#pragma once

#include "Graphics.h"

#include "CTL/RefPointer.h"

namespace Citrom::RenderAPI
{
	struct ShaderDesc
	{
		std::string name;
	};
	
	// Shaders internally have a vertex and fragment shader, and they get loaded based on name
	// and the data is acquired from ShaderCache/, not as modular as having a system that has
	// CreateShaderFromData, GetShaderDataFromFile, separated shader formats as well as
	// the renderer would be responsible for the shader data acquirement from files instead of the render api.
	struct Shader
	{
		ShaderDesc descriptor;
		CTL::Ref<void> internal;

		FORCE_INLINE bool IsShader(const std::string& name) const
		{
			return (descriptor.name == name);
		}
	};
	using ComputeShader = Shader;
	using ComputeShaderDesc = ShaderDesc;
}