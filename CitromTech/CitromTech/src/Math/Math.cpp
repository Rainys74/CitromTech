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
		const float32 halfInput = number * 0.5f; // (x2)
		float32 inverseSquareRoot = number; // (y)
		int32 inputBits = *reinterpret_cast<int32*>(&inverseSquareRoot); // Bit-level representation of the input number, those weird pointers are to avoid explicit casting (i)

		constexpr float32 factor = 1.5f; // (threehalfs)

		// Algorithm
		inputBits = 0x5f3759df - (inputBits >> 1); // dark magic
		inverseSquareRoot = *reinterpret_cast<float32*>(&inputBits); // apply the dark magic

		inverseSquareRoot = inverseSquareRoot * (factor - (halfInput * inverseSquareRoot * inverseSquareRoot)); // first iteration
		inverseSquareRoot = inverseSquareRoot * (factor - (halfInput * inverseSquareRoot * inverseSquareRoot)); // second iteration, technically improves the accuracy by a lot, without increasing the performance cost too much.

		return inverseSquareRoot;
	}

	float64 InverseSquareRoot(const float64 number)
	{
		return 1 / SquareRoot(number);
	}
}