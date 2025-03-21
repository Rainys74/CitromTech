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
	struct ActiveComponent // ToggleComponent
	{
		bool active;

		ActiveComponent() = default;
		ActiveComponent(const ActiveComponent&) = default;
	};
	struct HierarchyComponent
	{
		UUID parentID = UUID(0);

		HierarchyComponent() = default;
		HierarchyComponent(const HierarchyComponent&) = default;
	};

	struct TransformComponent
	{
		Math::Transform transform;
	};
}