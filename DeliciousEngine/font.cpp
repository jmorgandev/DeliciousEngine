#include "font.h"

#include "texture.h"
#include "shader.h"

Font::Font(Texture* texture_in, Shader* shader_in) {
	texture = texture_in;
	shader = shader_in;
}