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

		FORCE_INLINE void Rotate(float32 xRot, float32 yRot, float32 zRot) // Degrees
		{
			// TODO: either store the xDeg, yDeg, zDeg etc. or just call RotateInRadians.
			rotation = Quaternion::Euler(DegreesToRadians(xRot), DegreesToRadians(yRot), DegreesToRadians(zRot));
			eulerAnglesHint = Vector3(DegreesToRadians(xRot), DegreesToRadians(yRot), DegreesToRadians(zRot));
		}
		FORCE_INLINE void RotateInRadians(float32 xRad, float32 yRad, float32 zRad)
		{
			rotation = Quaternion::Euler(xRad, yRad, zRad);
			eulerAnglesHint = Vector3(xRad, yRad, zRad);
		}

		// Presets
		FORCE_INLINE Vector3 Forward() const { return rotation.Rotate(Vector3::Forward()); }
		FORCE_INLINE Vector3 Right() const { return rotation.Rotate(Vector3::Right()); }
		FORCE_INLINE Vector3 Up() const { return rotation.Rotate(Vector3::Up()); }

		// Operator Overloading for relative transformations
		Transform operator*(const Transform& trans2) const
		{
			Transform transform;
			transform.position = this->position + trans2.position;

			transform.rotation = this->rotation * trans2.rotation;
			//transform.eulerAnglesHint = this->eulerAnglesHint + trans2.eulerAnglesHint;

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