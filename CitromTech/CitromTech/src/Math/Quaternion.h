#pragma once

#include "MathCommon.h"
#include "Vector.h"

#include "CTL/StaticArray.h"

namespace Citrom::Math
{
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(const Quaternion&) = default;
		Quaternion(std::initializer_list<float32> values);
		~Quaternion() = default;

		Vector3 ToEulerAngles();

		static Quaternion Identity() { return Quaternion({0.0f, 0.0f, 0.0f, 1.0f}); }
		static Quaternion Euler(float eulerX, float eulerY, float eulerZ);

		/*
		 * Works because stack memory is stored contiguously,
		 * No need to account for byte sizes (sizeof() operator) because if the type is known
		 * the compiler already calculates it correctly
		*/
		FORCE_INLINE float32& operator[](size_t index) { return *(float32*)(&x + index); }
		FORCE_INLINE const float32& operator[](size_t index) const { return *(float32*)(&x + index); }
	public:
		union
		{
			struct
			{
				float32 x, y, z, w;
			};
			struct
			{
				float32 data[4];
			};
		};
	};
}