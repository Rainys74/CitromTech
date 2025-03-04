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
		//Material material;
		
		bool castShadows; // or use flags maybe?
	};
	struct CubeComponent // Box Component
	{
		//Material material;
		bool test;
	};

	/*enum class LightType
	{
		Directional
	};
	struct LightComponent
	{
		LightType type = LightType::Directional;

		// omit values from gui based on light type
	};*/

	/*struct ILightComponentBase
	{
		Math::ColorF32x3 color = Math::ColorF32x3(1.0f, 1.0f, 1.0f);
		float32 intensity = 1.0f;
	};

	using DirectionalLightComponent = ILightComponentBase;
	struct PointLightComponent : ILightComponentBase
	{
		float32 range = 10.0f;
	};
	struct SpotLightComponent : ILightComponentBase
	{
		float32 range = 10.0f;
		float32 spotAngle = 30.0f; // TODO: 30 degrees, however might need to use radians for this
	};*/
}