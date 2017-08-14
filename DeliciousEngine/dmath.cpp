#include "dmath.h"

unsigned abs(int value) { return value < 0 ? 0 - unsigned(value) : unsigned(value); }

namespace math {

	uint8 delta(uint8 a, uint8 b) {
		if (a > b) return a - b;
		else return b - a;
	}

	uint16 delta(uint16 a, uint16 b) {
		if (a > b) return a - b;
		else return b - a;
	}

	uint32 delta(uint32 a, uint32 b) {
		if (a > b) return a - b;
		else return b - a;
	}

	uint8 abs(int8 value) {
		return (value < 0) ? 0 - uint8(value) : uint8(value);
	}

	uint16 abs(int16 value) {
		return (value < 0) ? 0 - uint16(value) : uint16(value);
	}

	uint32 abs(int32 value)	{
		return (value < 0) ? 0 - uint32(value) : uint32(value);
	}

	float wrapf(float v, float min, float max) {
		if (v < min) return max;
		if (v > max) return min;
		return v;
	}
	int mem_diff(cstring first, cstring second)	{
		if (first > second) return first - second;
		else return second - first;
	}
}