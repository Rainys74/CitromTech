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

	float32 Sine(const float32 number)
	{
		return std::sinf(number);
	}
	float64 Sine(const float64 number)
	{
		return std::sin(number);
	}

	float32 Cosine(const float32 number)
	{
		return std::cosf(number);
	}
	float64 Cosine(const float64 number)
	{
		return std::cos(number);
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

	float32 FovXToFovY(float32 fovX, float32 aspect)
	{
		// tan(fovy / 2) = tan(fovx / 2) / aspect
		return 2.0f * std::atan(std::tan(fovX * 0.5f) / aspect);
	}
	float32 FovYToFovX(float32 fovY, float32 aspect)
	{
		// tan(fovx / 2) = tan(fovy / 2) * aspect
		return 2.0f * std::atan(std::tan(fovY * 0.5f) * aspect);
	}

	namespace AccuratePI
	{
		float64 Leibzniz(uint32fast terms)
		{
			/*double pi = 0.0;
			for (int i = 0; i < terms; i++) 
			{
				if (i % 2 == 0) 
				{
					pi += 1.0 / (2 * i + 1);
				}
				else 
				{
					pi -= 1.0 / (2 * i + 1);
				}
			}
			return pi * 4;*/

			float64 pi = 0.0;
			for (uint32fast i = 0; i < terms; i++)
			{
				if (i % 2 == 0)
				{
					pi += WHFSUF(1, 0) / SCast<float64>(2 * i + 1);
				}
				else
				{
					pi -= WHFSUF(1, 0) / SCast<float64>(2 * i + 1);
				}
			}
			return pi * WHFSUF(4, 0);
		}

		float64 Machin()
		{
			/*float64 pi = 4.0 * (4.0 * atan(1.0 / 5.0) - atan(1.0 / 239.0));
			return pi;*/
			return WHFSUF(4, 0) * (WHFSUF(4, 0) * atan(WHFSUF(1, 0) / WHFSUF(5, 0)) - atan(WHFSUF(1, 0) / WHFSUF(239, 0)));
		}

		// TODO: placeholder
		static float64 Random()
		{
			//static std::random_device rd;
			//static std::mt19937 gen(rd()); // maybe replace with mt19937_64 for more accurate results?
			//static std::uniform_real_distribution<float64> dis(0.0, 1.0);
			//return dis(gen);
			return 12554.35432189901;
		}

		float64 MonteCarlo(uint32fast samples)
		{
			/*
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0.0, 1.0);

			uint32fast insideCircle = 0;
			for (uint32fast i = 0; i < samples; i++)
			{
				float64 x = dis(gen);
				float64 y = dis(gen);
				if (x * x + y * y <= 1.0) 
				{
					insideCircle++;
				}
			}
			return 4.0 * insideCircle / samples;
			*/

			uint32fast insideCircle = 0;
			for (uint32fast i = 0; i < samples; i++)
			{
				float64 x = Random();
				float64 y = Random();
				if (x * x + y * y <= WHFSUF(1, 0))
				{
					insideCircle++;
				}
			}
			return WHFSUF(4, 0) * insideCircle / samples;
		}

		// TODO: Chudnovsky, AI e.g.
		/*
		#include <iostream>
		#include <cmath>
		#include <iomanip>
		#include <gmp.h>
		#include <mpfr.h>

		void chudnovskyPi(mpfr_t result, int digits) {
			mpfr_set_default_prec(8 * digits);

			mpfr_t sum, term, k_fact, six_k_fact, C, K, M, X;
			mpfr_inits2(8 * digits, sum, term, k_fact, six_k_fact, C, K, M, X, (mpfr_ptr) 0);

			mpfr_set_ui(sum, 0, MPFR_RNDN);
			mpfr_set_ui(k_fact, 1, MPFR_RNDN);
			mpfr_set_ui(six_k_fact, 1, MPFR_RNDN);
			mpfr_set_ui(C, 426880, MPFR_RNDN);
			mpfr_mul_ui(C, C, 10005, MPFR_RNDN);
			mpfr_sqrt(C, C, MPFR_RNDN);
			mpfr_set_ui(K, 6, MPFR_RNDN);

			for (int k = 0; k < digits; k++) {
				if (k > 0) {
					mpfr_mul_ui(k_fact, k_fact, k, MPFR_RNDN);
					mpfr_mul_ui(six_k_fact, six_k_fact, 6 * k, MPFR_RNDN);
				}

				mpfr_set_ui(M, 545140134, MPFR_RNDN);
				mpfr_mul_ui(M, M, k, MPFR_RNDN);
				mpfr_add_ui(M, M, 13591409, MPFR_RNDN);
				if (k % 2 != 0) {
					mpfr_neg(M, M, MPFR_RNDN);
				}

				mpfr_ui_pow_ui(X, 640320, 3 * k, MPFR_RNDN);
				mpfr_neg(X, X, MPFR_RNDN);

				mpfr_mul(term, M, six_k_fact, MPFR_RNDN);
				mpfr_div(term, term, X, MPFR_RNDN);
				mpfr_div(term, term, k_fact, MPFR_RNDN);
				mpfr_add(sum, sum, term, MPFR_RNDN);
			}

			mpfr_div(sum, C, sum, MPFR_RNDN);
			mpfr_set(result, sum, MPFR_RNDN);

			mpfr_clears(sum, term, k_fact, six_k_fact, C, K, M, X, (mpfr_ptr) 0);
		}

		int main() {
			int digits = 1000;
			mpfr_t pi;
			mpfr_init2(pi, 8 * digits);
			chudnovskyPi(pi, digits);

			std::cout << "Chudnovsky approximation of Pi: ";
			mpfr_out_str(stdout, 10, digits, pi, MPFR_RNDN);
			std::cout << std::endl;

			mpfr_clear(pi);
			return 0;
		}
		*/
	}
}