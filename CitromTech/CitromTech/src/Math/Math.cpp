#include "MathCommon.h"
#include "Vector.h"

#include <cmath>

namespace Citrom::Math
{
	float32 SquareRoot(const float32 number)
	{
		// algorithm example
		/*
		float32 y0 = 0.5f * number;
		float32 y1 = (y0 + number / y0) / 2;
		float32 y2 = (y1 + number / y1) / 2;
		return y2;
		*/
		// modular example
		/*
		float32 y = 0.5f * number;
		for (uint16fast i = 0; i < 52; i++)
		{
			y = (y + number / y) / 2;
		}
		*/

		// optimized version
		//CONSTEXPR uint8fast algorithmAccuracy = 52;
		//STATIC_ASSERT(algorithmAccuracy <= UINT8_MAX, "Algorithm Accuracy in math_square_rootf() is higher than the max value of uint8!");

		// seems like the lower it goes the more accurate it is
		// but for a couple of numbers it becomes less than the actual number
		// after a couple minutes of testing i think i found the sweet spot.. hopefully.
		float32 y = 0.19f * number;

		for (uint8fast i = 0; i < 5; i++)
		{
			y = (y + number / y) * 0.5f;
		}
		return y;
	}

	float64 SquareRoot(const float64 number)
	{
		return std::sqrt(number);
	}

	float32 InverseSquareRoot(const float32 number)
	{
		// TODO: remake this so there's no licensing troubles
		long i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y;						// evil floating point bit level hacking
		i = 0x5f3759df - (i >> 1);               // what the fuck?
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));   // 1st iteration
		y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, technically improves the accuracy by a lot, without increasing the performance cost too much.

		return y;
	}

	float64 InverseSquareRoot(const float64 number)
	{
		return 1 / SquareRoot(number);
	}

	// Vector3
	#define IS_NAN(x) ((x) != (x))

	// TODO: this is Vector 2!
	float32 Vector3::Magnitude()
	{
		return SquareRoot(this->x * this->x + this->y * this->y);
	}
	float32 Vector3::SquaredMagnitude()
	{
		return (this->x * this->x + this->y * this->y);
	}
	Vector3 Vector3::Normalized()
	{
		Vector3 normalized = { 0 };
		//normalized.x = this->x / vector2_magnitude(this);
		//normalized.y = this->y / vector2_magnitude(this);
		float32 magnitude = Magnitude();

		normalized.x = this->x / magnitude;
		normalized.y = this->y / magnitude;

		if (IS_NAN(normalized.x))
		{
			normalized.x = 0.0f;
		}
		if (IS_NAN(normalized.y))
		{
			normalized.y = 0.0f;
		}

		return normalized;
	}
}