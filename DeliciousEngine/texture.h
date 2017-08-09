#ifndef DELICIOUS_TEXTURE_H
#define DELICIOUS_TEXTURE_H

#include <GL/glew.h>
#include "dtypes.h"

struct Texture {
	GLuint gpu_id;
	uint16 width, height;
	uint8 bytes_per_pixel;
};
#endif