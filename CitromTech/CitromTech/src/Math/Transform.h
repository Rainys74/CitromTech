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

		Math::Vector3 eulerAnglesHint;

		Math::Matrix4x4& GetTransformMatrix()
		{
			//m_Matrix = Math::Matrix4x4::TRS(position, rotation, scale);
			//return m_Matrix;

			/*glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);*/
			//CT_VERBOSE("TRANSLATED: {}", Matrix4x4::Translate(Matrix4x4::Identity(), position).ToString());
			//CT_VERBOSE("ROTATEDFROMQUAT: {}", Matrix4x4::FromQuaternion(rotation).ToString());
			//CT_VERBOSE("SCALED: {}", Matrix4x4::Scale(Matrix4x4::Identity(), scale).ToString());
			//CT_WARN("TRANSLATED * ROTATEDFROMQUAT: {}", (Matrix4x4::Translate(Matrix4x4::Identity(), position) * Matrix4x4::FromQuaternion(rotation)).ToString());
			//CT_WARN("TRANSLATED * ROTATEDFROMQUAT * SCALED: {}", (Matrix4x4::Translate(Matrix4x4::Identity(), position) * Matrix4x4::FromQuaternion(rotation) * Matrix4x4::Scale(Matrix4x4::Identity(), scale)).ToString());

			//m_Matrix = Matrix4x4::Scale(Matrix4x4::Identity(), scale)
			//	* Matrix4x4::FromQuaternion(rotation)
			//	* Matrix4x4::Translate(Matrix4x4::Identity(), position);
			m_Matrix = Matrix4x4::Translate(Matrix4x4::Identity(), position)
				* Matrix4x4::FromQuaternion(rotation)
				* Matrix4x4::Scale(Matrix4x4::Identity(), scale);
			return m_Matrix;
		}
		Math::Matrix4x4 GetCameraViewFromTransform() const
		{
			return Matrix4x4::FromQuaternion(rotation)
				* Matrix4x4::Translate(Matrix4x4::Identity(), -position);
		}

		// Presets
		FORCE_INLINE Vector3 Forward() const { return rotation.Rotate(Vector3::Forward()); }
		FORCE_INLINE Vector3 Right() const { return rotation.Rotate(Vector3::Right()); }
		FORCE_INLINE Vector3 Up() const { return rotation.Rotate(Vector3::Up()); }

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