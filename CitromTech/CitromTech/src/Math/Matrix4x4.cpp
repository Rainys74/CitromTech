#include "Matrix4x4.h"
#include "MathCommon.h"

#include "Profiling/Profiler.h"

namespace Citrom::Math
{
#define FUNCTION_TO_MATRIX_TYPE(FOO, ARGS) FOO ## _LH_ZO ARGS // TODO: TEMPORARY!!

	Matrix4x4::Matrix4x4()
	{
	}

	Matrix4x4::Matrix4x4(float32 number)
	{
		for (uint32 i = 0; i < (m_Data.Length() * m_Data[i].Length()); i++)
		{
			/*
			* Column Major:
				uint32 j = idx / numRows;  // Compute column index
				uint32 i = idx % numRows;  // Compute row index
			* Row Major:
				uint32 i = idx / numCols;  // Compute row index
				uint32 j = idx % numCols;  // Compute column index
			*/

			//m_Data[i / 4][i % 4] = number; // Row Major
			m_Data[i % 4][i / 4] = number; // Column Major
		}
	}
	Matrix4x4::Matrix4x4(std::initializer_list<std::initializer_list<float32>> values)
	{
		auto rowIt = values.begin();
		for (int i = 0; i < 4 && rowIt != values.end(); ++i, ++rowIt)
		{
			auto colIt = rowIt->begin();
			for (int j = 0; j < 4 && colIt != rowIt->end(); ++j, ++colIt)
			{
				m_Data[i][j] = *colIt;
			}
		}
	}
	void Matrix4x4::Zero()
	{
		Memory::Zero(m_Data.Data(), sizeof(float32) * (4 * 4));
	}
	void Matrix4x4::Multiply(const Matrix4x4& mat4B)
	{
		CT_PROFILE_MEMBER_FUNCTION();

		float a00 = m_Data[0][0], a01 = m_Data[0][1], a02 = m_Data[0][2], a03 = m_Data[0][3],
			a10 = m_Data[1][0], a11 = m_Data[1][1], a12 = m_Data[1][2], a13 = m_Data[1][3],
			a20 = m_Data[2][0], a21 = m_Data[2][1], a22 = m_Data[2][2], a23 = m_Data[2][3],
			a30 = m_Data[3][0], a31 = m_Data[3][1], a32 = m_Data[3][2], a33 = m_Data[3][3],

			b00 = mat4B.Data()[0][0], b01 = mat4B.Data()[0][1], b02 = mat4B.Data()[0][2], b03 = mat4B.Data()[0][3],
			b10 = mat4B.Data()[1][0], b11 = mat4B.Data()[1][1], b12 = mat4B.Data()[1][2], b13 = mat4B.Data()[1][3],
			b20 = mat4B.Data()[2][0], b21 = mat4B.Data()[2][1], b22 = mat4B.Data()[2][2], b23 = mat4B.Data()[2][3],
			b30 = mat4B.Data()[3][0], b31 = mat4B.Data()[3][1], b32 = mat4B.Data()[3][2], b33 = mat4B.Data()[3][3];

		m_Data[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
		m_Data[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
		m_Data[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
		m_Data[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
		m_Data[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
		m_Data[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
		m_Data[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
		m_Data[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
		m_Data[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
		m_Data[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
		m_Data[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
		m_Data[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
		m_Data[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
		m_Data[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
		m_Data[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
		m_Data[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
	}
	Vector3 Matrix4x4::GetTranslation()
	{
		Vector3 result;

		// Row-Major
		result.x = m_Data[3][0];
		result.y = m_Data[3][1];
		result.z = m_Data[3][2];

		return result;
	}
	void Matrix4x4::Transpose()
	{
		CT_PROFILE_MEMBER_FUNCTION();

		/*for (uint32 i = 0; i < m_Data.Length(); i++)
		{
			for (uint32 j = 0; j < m_Data[i].Length(); j++)
			{
				// Swap m_Data[i][j] and m_Data[j][i]
				//float32 temp = m_Data[i][j];
				m_Data[i][j] = m_Data[j][i];
				//m_Data[j][i] = temp;

				// Swap using a compact in-place method
				//m_Data[i][j] = m_Data[i][j] + m_Data[j][i];
				//m_Data[j][i] = m_Data[i][j] - m_Data[j][i];
				//m_Data[i][j] = m_Data[i][j] - m_Data[j][i];
			}
		}*/
		Matrix4x4 result;

		//result.Data()[1][1] = m_Data[1][1];
		//result.Data()[1][2] = m_Data[2][1];
		//result.Data()[1][3] = m_Data[3][1];
		//result.Data()[1][4] = m_Data[4][1];
		//result.Data()[2][1] = m_Data[1][2];
		//result.Data()[2][2] = m_Data[2][2];
		//result.Data()[2][3] = m_Data[3][2];
		//result.Data()[2][4] = m_Data[4][2];
		//result.Data()[3][1] = m_Data[1][3];
		//result.Data()[3][2] = m_Data[2][3];
		//result.Data()[3][3] = m_Data[3][3];
		//result.Data()[3][4] = m_Data[4][3];
		//result.Data()[4][1] = m_Data[1][4];
		//result.Data()[4][2] = m_Data[2][4];
		//result.Data()[4][3] = m_Data[3][4];
		//result.Data()[4][4] = m_Data[4][4];

		for (uint32 i = 0; i < m_Data.Length(); i++)
		{
			for (uint32 j = 0; j < m_Data[i].Length(); j++)
			{
				result.Data()[j][i] = m_Data[i][j];
			}
		}

		*this = result;
	}
	void Matrix4x4::Orthographic(const float32 left, const float32 right, const float32 bottom, const float32 top, const float32 zNear, const float32 zFar)
	{
		FUNCTION_TO_MATRIX_TYPE(Orthographic, (left, right, bottom, top, zNear, zFar));
	}
	void Matrix4x4::Perspective(const float32 fovy, const float32 aspect, const float32 zNear, const float32 zFar)
	{
		//FUNCTION_TO_MATRIX_TYPE(Perspective)(fovy, aspect, zNear, zFar);
		FUNCTION_TO_MATRIX_TYPE(Perspective, (fovy, aspect, zNear, zFar));
	}

	void Matrix4x4::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
	{
		//Vector3 f, u, s;
		//
		//glm_vec3_sub(center, eye, f);
		//f = f.Normalized();
		//
		//glm_vec3_crossn(up, f, s);
		//glm_vec3_cross(f, s, u);
		//
		//m_Data[0][0] = s[0];
		//m_Data[0][1] = u[0];
		//m_Data[0][2] = f[0];
		//m_Data[1][0] = s[1];
		//m_Data[1][1] = u[1];
		//m_Data[1][2] = f[1];
		//m_Data[2][0] = s[2];
		//m_Data[2][1] = u[2];
		//m_Data[2][2] = f[2];
		//m_Data[3][0] = -glm_vec3_dot(s, eye);
		//m_Data[3][1] = -glm_vec3_dot(u, eye);
		//m_Data[3][2] = -glm_vec3_dot(f, eye);
		//m_Data[0][3] = m_Data[1][3] = m_Data[2][3] = 0.0f;
		//m_Data[3][3] = 1.0f;

		// LH
		Vector3 f = center - eye;
		f = f.Normalized();

		Vector3 s = up.Cross(f);
		s = s.Normalized();

		Vector3 u = f.Cross(s);

		m_Data[0][0] = s[0];
		m_Data[0][1] = u[0];
		m_Data[0][2] = f[0];
		m_Data[1][0] = s[1];
		m_Data[1][1] = u[1];
		m_Data[1][2] = f[1];
		m_Data[2][0] = s[2];
		m_Data[2][1] = u[2];
		m_Data[2][2] = f[2];
		m_Data[3][0] = -s.Dot(eye);
		m_Data[3][1] = -u.Dot(eye);
		m_Data[3][2] = -f.Dot(eye);
		m_Data[0][3] = m_Data[1][3] = m_Data[2][3] = 0.0f;
		m_Data[3][3] = 1.0f;

		// RH
		/*
		CGLM_ALIGN(8) vec3 f, u, s;

		glm_vec3_sub(center, eye, f);
		glm_vec3_normalize(f);

		glm_vec3_crossn(f, up, s);
		glm_vec3_cross(s, f, u);

		dest[0][0] = s[0];
		dest[0][1] = u[0];
		dest[0][2] =-f[0];
		dest[1][0] = s[1];
		dest[1][1] = u[1];
		dest[1][2] =-f[1];
		dest[2][0] = s[2];
		dest[2][1] = u[2];
		dest[2][2] =-f[2];
		dest[3][0] =-glm_vec3_dot(s, eye);
		dest[3][1] =-glm_vec3_dot(u, eye);
		dest[3][2] = glm_vec3_dot(f, eye);
		dest[0][3] = dest[1][3] = dest[2][3] = 0.0f;
		dest[3][3] = 1.0f;
		*/
	}

	Matrix4x4 Matrix4x4::Translate(const Matrix4x4& mat, const Vector3& vec3)
	{
		Matrix4x4 result(mat);
		
		// Create the translation matrix (no scaling or rotation)
		//result.m_Data[3][0] += vec3.x;  // translation in X
		//result.m_Data[3][1] += vec3.y;  // translation in Y
		//result.m_Data[3][2] += vec3.z;  // translation in Z

		//// Create a translation matrix (Homogeneous)
		//result.m_Data[0][3] = vec3.x;  // translation in X
		//result.m_Data[1][3] = vec3.y;  // translation in Y
		//result.m_Data[2][3] = vec3.z;  // translation in Z

		for (int i = 0; i < 4; ++i)
			result.m_Data[3][i] += (result.m_Data[0][i] * vec3.x +
				result.m_Data[1][i] * vec3.y +
				result.m_Data[2][i] * vec3.z);

		return result;
	}

	Matrix4x4 Matrix4x4::Scale(const Matrix4x4& mat, const Vector3& vec3)
	{
		Matrix4x4 result(mat);

		result.Data()[0][0] *= vec3.x; // Scale X
		result.Data()[1][1] *= vec3.y; // Scale Y
		result.Data()[2][2] *= vec3.z; // Scale Z

		return result;
	}

	static void /*_Intern_*/ TRS_Translation(Matrix4x4& result, const Vector3& position)
	{
#ifdef COLUMN_MAJOR
		// Translation Matrix (Column Major)
		result.Data()[0][3] = position.x;
		result.Data()[1][3] = position.y;
		result.Data()[2][3] = position.z;
#else

		// Translation Matrix (Row Major)
		result.Data()[3][0] = position.x;
		result.Data()[3][1] = position.y;
		result.Data()[3][2] = position.z;
#endif
	}
	static void TRS_Rotation(Matrix4x4& result, const Quaternion& rotation)
	{
		// Convert Quaternion to Rotation Matrix (Row-Major)
		float32 xx = rotation.x * rotation.x;
		float32 yy = rotation.y * rotation.y;
		float32 zz = rotation.z * rotation.z;
		float32 ww = rotation.w * rotation.w;

		float32 xy = rotation.x * rotation.y;
		float32 xz = rotation.x * rotation.z;
		float32 xw = rotation.x * rotation.w;
		float32 yz = rotation.y * rotation.z;
		float32 yw = rotation.y * rotation.w;
		float32 zw = rotation.z * rotation.w;

		result.Data()[0][0] = ww + xx - yy - zz;
		result.Data()[0][1] = 2.0f * (xy - zw);
		result.Data()[0][2] = 2.0f * (xz + yw);

		result.Data()[1][0] = 2.0f * (xy + zw);
		result.Data()[1][1] = ww - xx + yy - zz;
		result.Data()[1][2] = 2.0f * (yz - xw);

		result.Data()[2][0] = 2.0f * (xz - yw);
		result.Data()[2][1] = 2.0f * (yz + xw);
		result.Data()[2][2] = ww - xx - yy + zz;
	}
	static void TRS_Scaling(Matrix4x4& result, const Vector3& scale)
	{
		// Scale Matrix (Component-wise multiplication)
		result.Data()[0][0] *= scale.x;
		result.Data()[0][1] *= scale.x;
		result.Data()[0][2] *= scale.x;

		result.Data()[1][0] *= scale.y;
		result.Data()[1][1] *= scale.y;
		result.Data()[1][2] *= scale.y;

		result.Data()[2][0] *= scale.z;
		result.Data()[2][1] *= scale.z;
		result.Data()[2][2] *= scale.z;
	}

	Matrix4x4 Matrix4x4::TRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		Matrix4x4 result = Matrix4x4::Identity();

#if 1
		TRS_Translation(result, position);
		TRS_Rotation(result, rotation);
		TRS_Scaling(result, scale);
#else
		TRS_Scaling(result, scale);
		TRS_Rotation(result, rotation);
		TRS_Translation(result, position);
#endif

		return result;
	}
	Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& matB)
	{
		auto& a = matB.Data();

		auto s0 = a[0][0] * a[1][1] - a[1][0] * a[0][1];
		auto s1 = a[0][0] * a[1][2] - a[1][0] * a[0][2];
		auto s2 = a[0][0] * a[1][3] - a[1][0] * a[0][3];
		auto s3 = a[0][1] * a[1][2] - a[1][1] * a[0][2];
		auto s4 = a[0][1] * a[1][3] - a[1][1] * a[0][3];
		auto s5 = a[0][2] * a[1][3] - a[1][2] * a[0][3];

		auto c5 = a[2][2] * a[3][3] - a[3][2] * a[2][3];
		auto c4 = a[2][1] * a[3][3] - a[3][1] * a[2][3];
		auto c3 = a[2][1] * a[3][2] - a[3][1] * a[2][2];
		auto c2 = a[2][0] * a[3][3] - a[3][0] * a[2][3];
		auto c1 = a[2][0] * a[3][2] - a[3][0] * a[2][2];
		auto c0 = a[2][0] * a[3][1] - a[3][0] * a[2][1];

		// Should check for 0 determinant
		//auto invdet = 1.0 / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);

		auto det = (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
		if (std::abs(det) < 1e-6)
			return Matrix4x4::Identity();

		auto invdet = 1.0f / det;

		Matrix4x4 result;
		auto& b = result.Data();

		b[0][0] = (a[1][1] * c5 - a[1][2] * c4 + a[1][3] * c3) * invdet;
		b[0][1] = (-a[0][1] * c5 + a[0][2] * c4 - a[0][3] * c3) * invdet;
		b[0][2] = (a[3][1] * s5 - a[3][2] * s4 + a[3][3] * s3) * invdet;
		b[0][3] = (-a[2][1] * s5 + a[2][2] * s4 - a[2][3] * s3) * invdet;

		b[1][0] = (-a[1][0] * c5 + a[1][2] * c2 - a[1][3] * c1) * invdet;
		b[1][1] = (a[0][0] * c5 - a[0][2] * c2 + a[0][3] * c1) * invdet;
		b[1][2] = (-a[3][0] * s5 + a[3][2] * s2 - a[3][3] * s1) * invdet;
		b[1][3] = (a[2][0] * s5 - a[2][2] * s2 + a[2][3] * s1) * invdet;

		b[2][0] = (a[1][0] * c4 - a[1][1] * c2 + a[1][3] * c0) * invdet;
		b[2][1] = (-a[0][0] * c4 + a[0][1] * c2 - a[0][3] * c0) * invdet;
		b[2][2] = (a[3][0] * s4 - a[3][1] * s2 + a[3][3] * s0) * invdet;
		b[2][3] = (-a[2][0] * s4 + a[2][1] * s2 - a[2][3] * s0) * invdet;

		b[3][0] = (-a[1][0] * c3 + a[1][1] * c1 - a[1][2] * c0) * invdet;
		b[3][1] = (a[0][0] * c3 - a[0][1] * c1 + a[0][2] * c0) * invdet;
		b[3][2] = (-a[3][0] * s3 + a[3][1] * s1 - a[3][2] * s0) * invdet;
		b[3][3] = (a[2][0] * s3 - a[2][1] * s1 + a[2][2] * s0) * invdet;

		return result;
	}

	Matrix4x4 Matrix4x4::FromQuaternion(const Quaternion& quat)
	{
		//float xx = quat.x * quat.x;
		//float yy = quat.y * quat.y;
		//float zz = quat.z * quat.z;
		//float xy = quat.x * quat.y;
		//float xz = quat.x * quat.z;
		//float yz = quat.y * quat.z;
		//float wx = quat.w * quat.x;
		//float wy = quat.w * quat.y;
		//float wz = quat.w * quat.z;
		//
		//return Matrix4x4({
		//	1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz),       2.0f * (xz + wy),       0.0f,
		//	2.0f * (xy + wz),       1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx),       0.0f,
		//	2.0f * (xz - wy),       2.0f * (yz + wx),       1.0f - 2.0f * (xx + yy), 0.0f,
		//	0.0f,                   0.0f,                   0.0f,                   1.0f
		//});

		/*// Ensure quaternion is normalized (just in case)
		float32 length = SquareRoot(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
		if (length > 0) 
		{
			quat.x /= length;
			quat.y /= length;
			quat.z /= length;
			quat.w /= length;
		}*/

		Matrix4x4 m = Matrix4x4::Identity();

		const float32 sqw = quat.w * quat.w;
		const float32 sqx = quat.x * quat.x;
		const float32 sqy = quat.y * quat.y;
		const float32 sqz = quat.z * quat.z;

		// invs (inverse square length) is only required if quaternion is not already normalised
		const float32 invs = 1 / (sqx + sqy + sqz + sqw);
		m[0][0] = (sqx - sqy - sqz + sqw) * invs; // since sqw + sqx + sqy + sqz =1/invs*invs
		m[1][1] = (-sqx + sqy - sqz + sqw) * invs;
		m[2][2] = (-sqx - sqy + sqz + sqw) * invs;

		float32 tmp1 = quat.x * quat.y;
		float32 tmp2 = quat.z * quat.w;
		m[1][0] = 2.0 * (tmp1 + tmp2) * invs;
		m[0][1] = 2.0 * (tmp1 - tmp2) * invs;

		tmp1 = quat.x * quat.z;
		tmp2 = quat.y * quat.w;
		m[2][0] = 2.0 * (tmp1 - tmp2) * invs;
		m[0][2] = 2.0 * (tmp1 + tmp2) * invs;
		tmp1 = quat.y * quat.z;
		tmp2 = quat.x * quat.w;
		m[2][1] = 2.0 * (tmp1 + tmp2) * invs;
		m[1][2] = 2.0 * (tmp1 - tmp2) * invs;

		return m;
	}

	namespace glmRepl
	{
		template<typename T>
		static FORCE_INLINE T epsilon()
		{
			//if constexpr (std::is_same<T, float>::value)
			//{
			//	return static_cast<T>(1e-5); // epsilon for float
			//}
			//else if constexpr (std::is_same<T, double>::value)
			//{
			//	return static_cast<T>(1e-8); // epsilon for double
			//}
			//else
			//{
			//	return static_cast<T>(0); // for other types
			//}

			static_assert(std::numeric_limits<T>::is_iec559, "'epsilon' only accepts floating-point inputs");
			return std::numeric_limits<T>::epsilon();
		}

		template <typename T>
		static FORCE_INLINE bool epsilonEqual(T x, T y, T epsilon) { return false; }

		template<>
		static FORCE_INLINE bool epsilonEqual(float32 x, float32 y, float32 epsilon)
		{
			return std::abs(x - y) < epsilon;
		}

		template<typename T>
		static FORCE_INLINE bool epsilonNotEqual(T x, T y, T epsilon) { return false; }

		template <>
		static FORCE_INLINE bool epsilonNotEqual(float32 x, float32 y, float32 epsilon)
		{
			return std::abs(x - y) >= epsilon;
		}

		static float32 length(const Vector3& v)
		{
			return SquareRoot(v.x * v.x + v.y * v.y + v.z * v.z);
		}
		namespace detail
		{
			static Vector3 scale(const Vector3& v, float32 desiredLength)
			{
				//return v * desiredLength / length(v);
				auto len = length(v);
				return Vector3(
					v.x * desiredLength / len,
					v.y * desiredLength / len,
					v.z * desiredLength / len
				);
			}
		}
	}

	bool Matrix4x4::DecomposeTransform(const Matrix4x4& transform, Vector3& outTranslation, Vector3& outRotationEuler, Vector3& outScale)
	{
		// From glm::decompose in matrix_decompose.inl

		using namespace glmRepl;
		using T = float32;
		using length_t = size_t;

		Matrix4x4 LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of outTranslation (easy).
		//outTranslation = vec3(LocalMatrix[3]);
		outTranslation.x = LocalMatrix[3][0];
		outTranslation.y = LocalMatrix[3][1];
		outTranslation.z = LocalMatrix[3][2];

		//LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);
		LocalMatrix[3][0] = 0;
		LocalMatrix[3][1] = 0;
		LocalMatrix[3][2] = 0; 
		LocalMatrix[3][3] = LocalMatrix[3][3];

		Vector3 Row[3], Pdum3;

		// Now get outScale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X outScale factor and normalize first row.
		outScale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		outScale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		outScale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				outScale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		outRotationEuler.y = asin(-Row[0][2]);
		if (cos(outRotationEuler.y) != 0) 
		{
			outRotationEuler.x = atan2(Row[1][2], Row[2][2]);
			outRotationEuler.z = atan2(Row[0][1], Row[0][0]);
		}
		else 
		{
			outRotationEuler.x = atan2(-Row[2][0], Row[1][1]);
			outRotationEuler.z = 0;
		}

		return true;
	}
	void Matrix4x4::FlipHandedness()
	{
		//In RH systems, the near plane is closer to the viewer(z = 0), and the far plane is farther(z = 1).
		//In LH systems, the near plane is farther away(z = 0), and the far plane is closer(z = -1).

		// Reverse the sign of the z-axis
		for (uint32 row = 0; row < 4; ++row)
		{
			m_Data[row][2] = -m_Data[row][2]; // Flip the z-component of each row
		}
	}
	void Matrix4x4::ConvertDepth()
	{
		// Assuming m_Data represents the projection matrix (column-major or row-major as needed)
		// Swap the near and far planes to flip between ZO and ZN
		float32 near = m_Data[2][3];  // Get the near plane value from the matrix (usually at [2][3] in a perspective projection matrix)
		float32 far = m_Data[3][3];   // Get the far plane value (usually at [3][3])

		// Convert the near and far planes from ZO (positive depth) to ZN (negative depth)
		m_Data[2][3] = -far;  // Set the near plane to the negative of the far plane
		m_Data[3][3] = -near; // Set the far plane to the negative of the near plane

		/*
		// Reverse the process to convert from ZN (negative depth) back to ZO (positive depth)
		float32 near = m_Data[2][3];  // Get the near plane value from the matrix (usually at [2][3])
		float32 far = m_Data[3][3];   // Get the far plane value (usually at [3][3])

		// Convert the near and far planes from ZN (negative depth) to ZO (positive depth)
		m_Data[2][3] = -far;  // Set the near plane to the positive of the far plane
		m_Data[3][3] = -near; // Set the far plane to the positive of the near plane
		*/
	}

	Matrix4x4 Matrix4x4::Identity()
	{
		/*Matrix4x4 matrix;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				matrix.Data()[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}

		return matrix;*/
		// from 0.004300 ms (0.03% of 75fps) up to 0.001700 ms (0.01% of 75 fps)
		return Matrix4x4(
			{
				{1.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f},
			});
	}

	std::string Matrix4x4::ToString() const
	{
		std::string prettyString;
		for (uint32 i = 0; i < m_Data.Length(); i++) // Column
		{
			prettyString.append("[");
			for (uint32 j = 0; j < m_Data[i].Length(); j++) // Row
			{
				prettyString.append(std::to_string(m_Data[i][j]));

				if (j != m_Data[i].Length() - 1) // Check to avoid trailing comma
					prettyString.append(", ");
			}

			prettyString.append(" ]");

			// Add a newline after each column, except for the last one
			if (i != m_Data.Length() - 1)
				prettyString += "\n";
		}
		return prettyString;
	}

	void Matrix4x4::Perspective_LH_ZO(const float32 fovy, const float32 aspect, const float32 zNear, const float32 zFar)
	{
		Zero();

		const float32 f = 1.0f / tanf(fovy * 0.5f);
		const float32 fn = 1.0f / (zNear - zFar);

		m_Data[0][0] = f / aspect;
		m_Data[1][1] = f;
		m_Data[2][2] = -zFar * fn;
		m_Data[2][3] = 1.0f;
		m_Data[3][2] = zNear * zFar * fn;
	}

	void Matrix4x4::Perspective_RH_ZO(const float32 fovy, const float32 aspect, const float32 zNear, const float32 zFar)
	{
		Zero();

		const float32 f = 1.0f / tanf(fovy * 0.5f);
		const float32 fn = 1.0f / (zNear - zFar);

		m_Data[0][0] = f / aspect;
		m_Data[1][1] = f;
		m_Data[2][2] = zFar * fn;
		m_Data[2][3] = -1.0f;
		m_Data[3][2] = zNear * zFar * fn;
	}

	void Matrix4x4::Orthographic_LH_ZO(const float32 left, const float32 right, const float32 bottom, const float32 top, const float32 zNear, const float32 zFar)
	{
		Zero();

		const float32 rl = 1.0f / (right - left);
		const float32 tb = 1.0f / (top - bottom);
		const float32 fn = -1.0f / (zFar - zNear);

		m_Data[0][0] = 2.0f * rl;
		m_Data[1][1] = 2.0f * tb;
		m_Data[2][2] = -fn;
		m_Data[3][0] = -(right + left) * rl;
		m_Data[3][1] = -(top + bottom) * tb;
		m_Data[3][2] = zNear * fn;
		m_Data[3][3] = 1.0f;
	}

	void Matrix4x4::Orthographic_RH_ZO(const float32 left, const float32 right, const float32 bottom, const float32 top, const float32 zNear, const float32 zFar)
	{
		Zero();

		// TODO: RH_ZO (Right-Handed, Zero-Origin) is this ok?
		const float rl = 1.0f / (right - left);
		const float tb = 1.0f / (top - bottom);
		const float fn = -1.0f / (zFar - zNear);

		m_Data[0][0] = 2.0f * rl;
		m_Data[1][1] = 2.0f * tb;
		m_Data[2][2] = fn;
		m_Data[3][0] = -(right + left) * rl;
		m_Data[3][1] = -(top + bottom) * tb;
		m_Data[3][2] = zNear * fn;
		m_Data[3][3] = 1.0f;
	}
}