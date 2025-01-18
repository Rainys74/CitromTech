#include "Matrix4x4.h"

namespace Citrom::Math
{
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
	void Matrix4x4::Transpose()
	{
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
}