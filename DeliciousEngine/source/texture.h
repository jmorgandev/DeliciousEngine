#ifndef DELICIOUS_TEXTURE_H
#define DELICIOUS_TEXTURE_H

#include <glew.h>
#include <unordered_map>
#include "dtypes.h"

struct Texture {
	GLuint id;
	int width, height;
	int bytes_per_pixel;
};

#endif