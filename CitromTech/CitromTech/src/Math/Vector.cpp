#include "Vector.h"

namespace Citrom::Math
{
	// Vector2
	float32 Vector2::Magnitude() const
	{
		return SquareRoot(this->x * this->x + this->y * this->y);
	}
	float32 Vector2::SquaredMagnitude() const
	{
		return (this->x * this->x + this->y * this->y);
	}
	Vector2 Vector2::Normalized() const
	{
		Vector2 normalized = {0};
		float32 magnitude = Magnitude();

		normalized.x = this->x / magnitude;
		normalized.y = this->y / magnitude;

		if (IS_NAN(normalized.x))
		{
			normalized.x = 0.0f;
		}
		if (IS_NAN(normalized.y))
		{
			normalized.y = 0.0f;
		}

		return normalized;
	}

	float32 Vector3::Magnitude() const
	{
		return SquareRoot(this->x * this->x + this->y * this->y + this->z * this->z);
	}
	float32 Vector3::SquaredMagnitude() const
	{
		return (this->x * this->x + this->y * this->y + this->z * this->z);
	}
	Vector3 Vector3::Normalized() const
	{
		Vector3 normalized;
		float32 magnitude = Magnitude();

		normalized.x = this->x / magnitude;
		normalized.y = this->y / magnitude;
		normalized.z = this->z / magnitude;

		if (IS_NAN(normalized.x))
			normalized.x = 0.0f;
		if (IS_NAN(normalized.y))
			normalized.y = 0.0f;
		if (IS_NAN(normalized.z))
			normalized.z = 0.0f;

		return normalized;
	}
	float32 Vector3::Dot(const Vector3& vec3B) const
	{
		return this->x * vec3B.x + this->y * vec3B.y + this->z * vec3B.z;
	}
	Vector3 Vector3::Cross(const Vector3& vec3B) const
	{
		return Vector3{ 
			this->y * vec3B.z - this->z * vec3B.y, // X
			this->z * vec3B.x - this->x * vec3B.z, // Y
			this->x * vec3B.y - this->y * vec3B.x  // Z
		};
	}
}