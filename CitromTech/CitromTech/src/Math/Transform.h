#pragma once

#include "MathCommon.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

namespace Citrom::Math
{
	class Transform
	{
	public:
		Math::Vector3 position;
		Math::Quaternion rotation;
		Math::Vector3 scale;

		const Math::Matrix4x4& GetTransformMatrix() const
		{
			//m_Matrix = Matrix4x4::TRS();
			return m_Matrix;
		}

		// Operator Overloading for relative transformations
		Transform operator*(const Transform& trans2) const
		{
			Transform transform;
			transform.position.x = this->position.x + trans2.position.x;
			transform.position.y = this->position.y + trans2.position.y;
			transform.position.z = this->position.z + trans2.position.z;

			return transform;
		}
	private:
		Math::Matrix4x4 m_Matrix = Math::Matrix4x4::Identity();
	};
}