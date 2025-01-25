#pragma once

#include "Core.h"

#include "CitromUUID.h"
#include "Math/Transform.h"

#include "CTL/String.h"

namespace Citrom
{
	struct UUIDComponent
	{
		UUID id;

		UUIDComponent() = default;
		UUIDComponent(const UUIDComponent&) = default;
	};
	struct NameComponent
	{
		std::string name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
	};

	struct TransformComponent
	{
		Math::Transform transform;
	};
}