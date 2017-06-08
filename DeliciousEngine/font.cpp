#include "font.h"

#include "texture.h"
#include "shader.h"

Font::Font(Texture* texture_in, Shader* shader_in) {
	texture = texture_in;
	shader = shader_in;
}

Texture* Font::get_texture() {
	return texture;
}

Shader* Font::get_shader() {
	return shader;
}