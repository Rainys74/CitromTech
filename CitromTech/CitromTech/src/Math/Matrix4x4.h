#pragma once

#include "MathCommon.h"

#include "CTL/StaticArray.h"

namespace Citrom::Math
{
	// Column Major 4x4 Matrix for GPU efficiency
	// is stored in memory as:	(00, 10, 20, 30),
	//							(01, 11, 21, 31),
	//							(02, 12, 22, 32),
	//							(03, 13, 23, 33)
	class Matrix4x4
	{
	public:
		Matrix4x4();
		Matrix4x4(float32 number);
		Matrix4x4(const Matrix4x4&) = default;
		Matrix4x4(std::initializer_list<std::initializer_list<float32>> values);
		~Matrix4x4() = default;

		void Transpose();

		// TODO: test out if these work
		void FlipHandedness(); // Reverses LH to RH matrices and vice-versa
		void ConvertDepth(); // Changes ZO to ZN and vice-versa

		// Should only be used for debugging purposes
		std::string ToString();

		FORCE_INLINE CTL::Array<CTL::Array<float32, 4>, 4>& Data() { return m_Data; }
	private:
		CTL::Array<CTL::Array<float32, 4>, 4> m_Data;
	};
}