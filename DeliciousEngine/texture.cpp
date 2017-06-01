#include "texture.h"

Texture::Texture (GLuint object_in, uint16 width_in, uint16 height_in, uint8 bpp_in) {
	gpu_object = object_in;
	width = width_in;
	height = height_in;
	bpp = bpp_in;
}
Texture::~Texture() {
	glDeleteTextures(1, &gpu_object);
}

void Texture::bind() {
	
}

GLuint Texture::get_gpu_object() {
	return gpu_object;
}

uint16 Texture::get_width() {
	return width;
}

uint16 Texture::get_height() {
	return height;
}

uint8 Texture::get_bpp() {
	return bpp;
}