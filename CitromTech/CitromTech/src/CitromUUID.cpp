#include "CitromUUID.h"

//#include "Platform/Platform.h"

#include <random>

namespace Citrom
{
	static std::mt19937_64 s_RandomEngine(std::random_device{}());
	static std::uniform_int_distribution<uint64> s_Distribution;

	UUID::UUID()
		: m_ID(s_Distribution(s_RandomEngine) /** (uint64)Platform::Utils::GetProcessID()*/)
	{
	}

	UUID::UUID(uint64 uuid)
		: m_ID(uuid)
	{
	}
}