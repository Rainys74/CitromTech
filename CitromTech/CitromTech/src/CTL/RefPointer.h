#pragma once

#include "CTL.h"

#include <memory>

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
    // Reference Counted Pointer wrapper, also known as
    // Shared Ptr (C++)
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}