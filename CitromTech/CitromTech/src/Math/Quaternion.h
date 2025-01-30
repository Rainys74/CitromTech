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
		static Quaternion Euler(const Vector3& vec3);

		FORCE_INLINE Quaternion Conjugate() const { return Quaternion({ -x, -y, -z, w }); }

		inline Quaternion operator*(const Quaternion& q) const
		{
			return Quaternion({
				w * q.x + x * q.w + y * q.z - z * q.y,
				w * q.y - x * q.z + y * q.w + z * q.x,
				w * q.z + x * q.y - y * q.x + z * q.w,
				w * q.w - x * q.x - y * q.y - z * q.z
			});
		}

		inline Vector3 operator*(const Vector3& v) const
		{
			Quaternion qv({ v.x, v.y, v.z, 0.0f }); // Convert vector to quaternion
			Quaternion res = (*this) * qv * this->Conjugate(); // q * v * q^-1
			return Vector3(res.x, res.y, res.z); // Extract rotated vector
		}

		// Should only be used for debugging purposes
		std::string ToString() const;

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