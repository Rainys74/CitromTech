#pragma once

#include "MathCommon.h"
#include "Vector.h"
#include "Quaternion.h"

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
		using DataContainerType = CTL::Array<CTL::Array<float32, 4>, 4>;
	public:
		Matrix4x4();
		Matrix4x4(float32 number);
		Matrix4x4(const Matrix4x4&) = default;
		Matrix4x4(std::initializer_list<std::initializer_list<float32>> values);
		Matrix4x4& operator=(const Matrix4x4&) = default;
		~Matrix4x4() = default;

		void Zero();
		void Multiply(const Matrix4x4& mat4B);

		void Transpose();
		void Orthographic(const float32 left, const float32 right, const float32 bottom, const float32 top, const float32 zNear = 0.0f, const float32 zFar = 1.0f);
		void Perspective(const float32 fovy, const float32 aspect, const float32 zNear, const float32 zFar);

		static Matrix4x4 Translate(const Matrix4x4& mat, const Vector3& vec3);
		static Matrix4x4 TRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

		// TODO: test out if these work
		void FlipHandedness(); // Reverses LH to RH matrices and vice-versa
		void ConvertDepth(); // Changes ZO to ZN (NO) and vice-versa

		static Matrix4x4 Identity();
		//inline static Matrix4x4 IdentityFast() { return Matrix4x4({ {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} }); }

		// Should only be used for debugging purposes
		std::string ToString();

		Matrix4x4 operator*(const Matrix4x4& mat4B) const { Matrix4x4 result = *this; result.Multiply(mat4B); return result; }

		FORCE_INLINE CTL::Array<CTL::Array<float32, 4>, 4>& Data() { return m_Data; }
		FORCE_INLINE const CTL::Array<CTL::Array<float32, 4>, 4>& Data() const { return m_Data; }
	private:
		void Perspective_LH_ZO(const float32 fovy, const float32 aspect, const float32 zNear, const float32 zFar); // DirectX 11/12, Vulkan, Metal
		void Perspective_RH_ZO(const float32 fovy, const float32 aspect, const float32 zNear, const float32 zFar); // OpenGL (modern w/ Vulkan-style depth)

		void Orthographic_LH_ZO(const float32 left, const float32 right, const float32 bottom, const float32 top, const float32 zNear, const float32 zFar); // DirectX 11/12, Vulkan, Metal
		void Orthographic_RH_NO(const float32 left, const float32 right, const float32 bottom, const float32 top, const float32 zNear, const float32 zFar){} // OpenGL (legacy)
		void Orthographic_RH_ZO(const float32 left, const float32 right, const float32 bottom, const float32 top, const float32 zNear, const float32 zFar); // OpenGL (modern w/ Vulkan-style depth)
	private:
		CTL::Array<CTL::Array<float32, 4>, 4> m_Data;
	};
}