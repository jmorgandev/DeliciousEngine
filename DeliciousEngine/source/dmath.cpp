#include "dmath.h"

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

	int32 min(int32 value, int32 min) {
		if (value < min) return min;
		return value;
	}

	float sine(float magnitude, float time) {
		return sinf(time) * magnitude;
	}

	float sine(float magnitude, float frequency, float time) {
		return sinf(time * frequency) * magnitude;
	}

	float cosine(float magnitude, float time) {
		return cosf(time) * magnitude;
	}

	float cosine(float magnitude, float frequency, float time) {
		return cosf(time * frequency) * magnitude;
	}

	float lerp(float start, float end, float percent) {
		return (start + percent * (end - start));
	}
}