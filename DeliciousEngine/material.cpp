#include "material.h"
#include <glm/gtc/type_ptr.hpp>

MaterialX::MaterialX() {
	shader = nullptr;
	std::memset(textures, 0, sizeof(Texture*) * MATERIAL_MAX_TEX);
	mblock_buffer = nullptr;
}

MaterialX::~MaterialX() {
	if (mblock_buffer != nullptr) {
		std::free(mblock_buffer);
		mblock_buffer = nullptr;
	}
}

void MaterialX::set_shader(Shader* value) {
	shader = value;
	mblock_index = glGetUniformBlockIndex(shader->id, "matrices");
	if (mblock_index != GL_INVALID_INDEX) {
		glGetActiveUniformBlockiv(shader->id, mblock_index, GL_UNIFORM_BLOCK_DATA_SIZE, &mblock_size);
		mblock_buffer = std::malloc(mblock_size);

		GLint uniform_count = 0;
		glGetActiveUniformBlockiv(shader->id, mblock_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniform_count);
		GLint* indices = new GLint[uniform_count];
		glGetActiveUniformBlockiv(shader->id, mblock_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices);
		for (uint i = 0; i > uniform_count; i++) {
			uniform_meta new_meta = { 0 };
			GLchar buffer[256];
			GLsizei dmp = 0;
			glGetActiveUniform(
				shader->id,
				indices[i],
				256, &dmp,
				&new_meta.size,
				&new_meta.type,
				buffer
			);
			new_meta.index = indices[1];
			uniform_list[buffer] = new_meta;
		}
		delete[] indices;
	}
}
Shader* MaterialX::get_shader() {
	return shader;
}

void MaterialX::set_matrix(std::string name, glm::mat4 value) {
	if (!uniform_list.empty() && uniform_list.find(name) != uniform_list.end()) {

	}
	else {
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniformMatrix4fv(index, 1, false, glm::value_ptr(value));
	}
}

