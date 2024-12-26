#pragma once

constexpr const char* GetBuildType()
{
#if   defined(CT_DEBUG)
	return "Debug";
#elif defined(CT_RELEASE)
	return "Release";
#elif defined(CT_OPTIMIZATION)
	return "Optimization";
#else
	return nullptr;
#endif
}