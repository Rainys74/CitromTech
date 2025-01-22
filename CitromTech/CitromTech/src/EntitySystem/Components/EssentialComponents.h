#pragma once

#include "Core.h"

#include "CitromUUID.h"
#include "Math/Transform.h"

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
		Math::Transform transform;
	};
}