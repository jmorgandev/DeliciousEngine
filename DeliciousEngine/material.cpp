#include "material.h"

MaterialX::MaterialX() {
	shader = nullptr;
	std::memset(textures, 0, sizeof(Texture*) * MATERIAL_MAX_TEX);
}

void MaterialX::set_shader(Shader* value) {
	shader = value;
	mblock_index = glGetUniformBlockIndex(shader->id, "matrices");
	if (mblock_index != GL_INVALID_INDEX) {
		mblock_buffer 
	}
}
Shader* MaterialX::get_shader() {
	return shader;
}

