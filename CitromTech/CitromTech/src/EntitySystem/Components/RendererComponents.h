#pragma once

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Renderer/MaterialSystem.h"

namespace Citrom
{
	struct CameraComponent
	{
		Camera camera;
	};
	struct ModelComponent
	{
		Mesh mesh;
		Material material;
		
		bool castShadows; // or use flags maybe?
	};
	struct CubeComponent // Box Component
	{
		Material material;
	};
}