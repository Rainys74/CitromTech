#include "Matrix4x4.h"

#include "Profiling/Profiler.h"

namespace Citrom::Math
{
#define FUNCTION_TO_MATRIX_TYPE(FOO, ARGS) FOO ## _LH_ZO ## ARGS // TODO: TEMPORARY!!

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

	std::string Matrix4x4::ToString()
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