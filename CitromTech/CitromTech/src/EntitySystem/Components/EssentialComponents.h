#pragma once

#include "Core.h"

#include "CitromUUID.h"
#include "Math/Vector.h"

namespace Citrom
{
	class UUIDComponent
	{
	public:
		UUID id;

		UUIDComponent() = default;
		UUIDComponent(const UUIDComponent&) = default;
	};

	class TransformComponent
	{
	public:
		Math::Vector3 position;
		Math::Vector3 eulerRotation;
		Math::Vector3 scale;
	};
}