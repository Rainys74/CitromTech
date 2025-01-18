#pragma once

#include "Core.h"

namespace Citrom
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64 uuid);
		~UUID() = default;

		UUID(const UUID&) = default;

		FORCE_INLINE operator uint64() const { return m_ID; }
	private:
		uint64 m_ID = 0;
	};
}