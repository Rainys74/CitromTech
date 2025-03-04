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

		Vector3 GetTranslation();

		void Transpose();
		void Orthographic(const float32 left, const float32 right, const float32 bottom, const float32 top, const float32 zNear = 0.0f, const float32 zFar = 1.0f);
		void Perspective(const float32 fovy, const float32 aspect, const float32 zNear, const float32 zFar);

		void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

		static Matrix4x4 Translate(const Matrix4x4& mat, const Vector3& vec3);
		static Matrix4x4 Scale(const Matrix4x4& mat, const Vector3& vec3);
		static Matrix4x4 TRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
		static Matrix4x4 Inverse(const Matrix4x4& matB);

		static Matrix4x4 FromQuaternion(const Quaternion& quat);

		static bool DecomposeTransform(const Matrix4x4& transform, Vector3& outTranslation, Vector3& outRotationEuler, Vector3& outScale);

		// TODO: test out if these work
		void FlipHandedness(); // Reverses LH to RH matrices and vice-versa
		void ConvertDepth(); // Changes ZO to ZN (NO) and vice-versa

		static Matrix4x4 Identity();
		//inline static Matrix4x4 IdentityFast() { return Matrix4x4({ {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} }); }

		// Should only be used for debugging purposes
		std::string ToString() const;

		Matrix4x4 operator*(const Matrix4x4& mat4B) const { Matrix4x4 result = *this; result.Multiply(mat4B); return result; }

		Vector3 operator*(const Vector3& vec)
		{
			const Matrix4x4& mat = *this;

			// Homogeneous coordinates for the vector (w = 1)
#ifdef COLUMN_MAJOR
			float x = vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0] + mat[3][0];
			float y = vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1] + mat[3][1];
			float z = vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2] + mat[3][2];
#else
			float x = vec.x * mat[0][0] + vec.y * mat[0][1] + vec.z * mat[0][2] + mat[0][3];
			float y = vec.x * mat[1][0] + vec.y * mat[1][1] + vec.z * mat[1][2] + mat[1][3];
			float z = vec.x * mat[2][0] + vec.y * mat[2][1] + vec.z * mat[2][2] + mat[2][3];
#endif

			return Vector3(x, y, z);
		}

		// Operator overloading
		//FORCE_INLINE Vector4& operator[](size_t index) { return &m_Data[index].Data(); }
		//FORCE_INLINE const Vector4& operator[](size_t index) const { return &m_Data[index].Data(); }

		FORCE_INLINE const float32* operator[](size_t row) const { return m_Data[row].Data(); }
		FORCE_INLINE float32* operator[](size_t row) { return m_Data[row].Data(); }

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