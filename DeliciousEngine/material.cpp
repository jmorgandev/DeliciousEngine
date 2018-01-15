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
	mblock_index = glGetUniformBlockIndex(shader->id, "BlobSettings");
	if (mblock_index != GL_INVALID_INDEX) {
		glGetActiveUniformBlockiv(shader->id, mblock_index, GL_UNIFORM_BLOCK_DATA_SIZE, &mblock_size);
		mblock_buffer = (GLubyte*)std::malloc(mblock_size);

		GLint uniform_count = 0;
		glGetActiveUniformBlockiv(shader->id, mblock_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniform_count);
		
		GLuint* indices = new GLuint[uniform_count];
		GLint* offsets = new GLint[uniform_count];
		GLint* types = new GLint[uniform_count];
		GLint* sizes = new GLint[uniform_count];

		glGetActiveUniformBlockiv(shader->id, mblock_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)indices);
		glGetActiveUniformsiv(shader->id, uniform_count, indices, GL_UNIFORM_OFFSET, offsets);
		glGetActiveUniformsiv(shader->id, uniform_count, indices, GL_UNIFORM_TYPE, types);
		glGetActiveUniformsiv(shader->id, uniform_count, indices, GL_UNIFORM_SIZE, sizes);
		for (uint i = 0; i < uniform_count; i++) {
			GLchar buffer[256];
			GLsizei chars_wrote = 0;
			glGetActiveUniformName(shader->id, indices[i], 256, &chars_wrote, buffer);
			uniform_meta new_uniform = {
				indices[i],
				offsets[i],
				types[i],
				sizes[i]
			};
			uniform_list[buffer] = new_uniform;
		}
		delete[] indices;
		delete[] offsets;
		delete[] types;
		delete[] sizes;
		
		glGenBuffers(1, &mblock_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, mblock_ubo);
		glBufferData(GL_UNIFORM_BUFFER, mblock_size, mblock_buffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, mblock_index, mblock_ubo);
	}
	else {
		assert(false);
	}
}
Shader* MaterialX::get_shader() {
	return shader;
}

void MaterialX::set_matrix(std::string name, glm::mat4 value) {
	auto it = uniform_list.find(name);
	if (it != uniform_list.end()) {
		uniform_meta meta = it->second;
		assert(meta.size == sizeof(glm::mat4));
		std::memcpy(mblock_buffer + meta.offset, glm::value_ptr(value), meta.size);
	}
	else {
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniformMatrix4fv(index, 1, false, glm::value_ptr(value));
	}
}

void MaterialX::set_vector4(std::string name, glm::vec4 value) {
	auto it = uniform_list.find(name);
	if (it != uniform_list.end()) {
		uniform_meta meta = it->second;
		//assert(meta.size == sizeof(glm::vec4));
		std::memcpy(mblock_buffer + meta.offset, glm::value_ptr(value), sizeof(glm::vec4));
	}
	else {
		glUseProgram(shader->id);
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniform4fv(index, 1, glm::value_ptr(value));
	}
}

void MaterialX::set_float(std::string name, GLfloat value) {
	auto it = uniform_list.find(name);
	if (it != uniform_list.end()) {
		uniform_meta meta = it->second;
		//assert(meta.size == sizeof(GLfloat));
		std::memcpy(mblock_buffer + meta.offset, &value, sizeof(GLfloat));
	}
	else {
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniform1f(index, value);
	}
}

void MaterialX::set_floatv(std::string name, GLfloat* values, GLuint size) {
	auto it = uniform_list.find(name);
	if (it != uniform_list.end()) {
		uniform_meta meta = it->second;
		//assert(meta.size == sizeof(GLfloat));
		std::memcpy(mblock_buffer + meta.offset, values, size * sizeof(GLfloat));
	}
	else {
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniform1fv(index, size, values);
	}
}

void MaterialX::bind() {
	//@SPEED Get buffer block binding point, bind program AND stream buffer data to
	// GPU. In future, group renderable objects by shader program so that
	// buffer binding point remains the same throughout all material instance binds
	// so that only the data needs to be streamed.

	//@SPEED glBufferData, glBufferSubData, glMapBuffer, glBindBufferRange.
	// call glBindBufferSubData for every uniform update?

	//@SPEED send important matrices through attributes rather than shaders?
	if (shader != nullptr) {
		glUseProgram(shader->id);
		glBindBuffer(GL_UNIFORM_BLOCK, mblock_ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, mblock_size, mblock_buffer);
	}
}

