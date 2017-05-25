#ifndef DELICIOUS_TEXTURE_H
#define DELICIOUS_TEXTURE_H

#include <GL/glew.h>
#include "dtypes.h"

class Texture {
public:
	Texture(GLuint object_in, uint16 width_in, uint16 height_in, uint8 bpp_in);
	~Texture();

	void	bind();

	GLuint	get_gpu_object();
	uint16	get_width();
	uint16	get_height();
	uint8	get_bpp();
private:
	GLuint	gpu_object;
	uint16	width, height;
	uint8	bpp;
};

#endif