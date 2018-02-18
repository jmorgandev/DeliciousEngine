#ifndef DELICIOUS_MATH_H
#define DELICIOUS_MATH_H

#include "dtypes.h"

#include <math.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

namespace math {
	uint8 delta(uint8 a, uint8 b);
	uint16 delta(uint16 a, uint16 b);
	uint32 delta(uint32 a, uint32 b);

	uint8 abs(int8 value);
	uint16 abs(int16 value);
	uint32 abs(int32 value);

	int32 min(int32 value, int32 min);

	float sine(float magnitude, float time);
	float sine(float magnitude, float frequency, float time);

	float cosine(float magnitude, float time);
	float cosine(float magnitude, float frequency, float time);
}

#endif