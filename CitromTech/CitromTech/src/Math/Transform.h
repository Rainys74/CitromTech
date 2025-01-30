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
		Math::Quaternion rotation = Math::Quaternion::Identity();
		Math::Vector3 scale = Math::Vector3(1.0f, 1.0f, 1.0f);

		Math::Vector3 editorEulerAngles; // Only for the editor.

		Math::Matrix4x4& GetTransformMatrix()
		{
			m_Matrix = Math::Matrix4x4::TRS(position, rotation, scale);
			return m_Matrix;
		}

		// Operator Overloading for relative transformations
		Transform operator*(const Transform& trans2) const
		{
			Transform transform;
			transform.position.x = this->position.x + trans2.position.x;
			transform.position.y = this->position.y + trans2.position.y;
			transform.position.z = this->position.z + trans2.position.z;

			// TODO: multiply rotation quaternions

			transform.scale = this->scale * trans2.scale;

			return transform;
		}

		// Should only be used for debugging purposes
		std::string ToString() const
		{
			return std::string("TO BE IMPLEMENTED!");
		}
	private:
		Math::Matrix4x4 m_Matrix = Math::Matrix4x4::Identity();
	};
}