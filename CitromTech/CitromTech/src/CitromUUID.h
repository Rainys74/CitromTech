#pragma once

#include "Core.h"

#include "CTL/HashMap.h"

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

		FORCE_INLINE bool operator==(const UUID& other) const { return m_ID == other.m_ID; }
		FORCE_INLINE bool operator==(const uint64 other) const { return m_ID == other; }

		FORCE_INLINE bool operator!=(const UUID& other) const { return m_ID != other.m_ID; }
		FORCE_INLINE bool operator!=(const uint64 other) const { return m_ID != other; }
	private:
		uint64 m_ID = 0;
	};
}

// CTL Hash Map Specialization Specification
namespace CTL
{
	template<>
	class XXHash<Citrom::UUID>
	{
	public:
		size_t operator()(const Citrom::UUID& uuid) const
		{
			return XXHash<uint64>()(static_cast<uint64>(uuid));
		}
	};
}