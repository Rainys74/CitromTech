#pragma once

#include "Renderer/Renderer.h"

namespace Citrom
{
	struct ModelComponent
	{
		Mesh mesh;
		Material material;
	};
	struct CubeComponent // Box Component
	{
		Material material;
	};
}