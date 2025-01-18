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

		void Multiply(const Matrix4x4& mat4B);

		void Transpose();
		void Orthographic(const float32 left, const float32 right, const float32 bottom, const float32 top, const float32 zNear = 0.0f, const float32 zFar = 1.0f);

		// TODO: test out if these work
		void FlipHandedness(); // Reverses LH to RH matrices and vice-versa
		void ConvertDepth(); // Changes ZO to ZN and vice-versa

		static Matrix4x4 Identity();
		//inline static Matrix4x4 IdentityFast() { return Matrix4x4({ {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} }); }

		// Should only be used for debugging purposes
		std::string ToString();

		FORCE_INLINE CTL::Array<CTL::Array<float32, 4>, 4>& Data() { return m_Data; }
		FORCE_INLINE const CTL::Array<CTL::Array<float32, 4>, 4>& Data() const { return m_Data; }
	private:
		CTL::Array<CTL::Array<float32, 4>, 4> m_Data;
	};
}