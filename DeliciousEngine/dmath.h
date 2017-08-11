#ifndef DELICIOUS_MATH_H
#define DELICIOUS_MATH_H

#include "dtypes.h"

namespace math {
	uint8 delta(uint8 a, uint8 b);
	uint16 delta(uint16 a, uint16 b);

	uint8 abs(int8 value);
	uint16 abs(int16 value);

	float wrapf(float v, float min, float max);
}

#endif