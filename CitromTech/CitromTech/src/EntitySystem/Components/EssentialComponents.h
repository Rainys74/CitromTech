#pragma once

#include "Core.h"

#include "CitromUUID.h"
#include "Math/Vector.h"

namespace Citrom
{
	struct UUIDComponent
	{
		UUID id;

		UUIDComponent() = default;
		UUIDComponent(const UUIDComponent&) = default;
	};

	struct TransformComponent
	{
		// TODO: figure out how to handle this
		Math::Vector3 position;
		Math::Vector3 eulerRotation;
		Math::Vector3 scale;
	};
}