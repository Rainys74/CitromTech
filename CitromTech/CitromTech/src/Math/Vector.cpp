#include "Vector.h"

#define IS_NAN(x) ((x) != (x))

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
		//normalized.x = vec2->x / vector2_magnitude(vec2);
		//normalized.y = vec2->y / vector2_magnitude(vec2);
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
}