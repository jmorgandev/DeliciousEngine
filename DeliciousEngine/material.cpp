#include "material.h"
#include <glm/gtc/type_ptr.hpp>

Material::Material(Shader* shader_program, std::string user_block) {
	block_buffer = nullptr;
	update_buffer = false;
	std::memset(sampler2d, 0, sizeof(GLenum) * MAX_TEXTURES);
	shader = shader_program;

	get_user_block(user_block);
	get_default_block();
}

Material::~Material() {
	if (block_buffer != nullptr) {
		std::free(block_buffer);
		block_buffer = nullptr;
	}
}

void Material::get_block(std::string name) {
	//@TODO: Some block shit to do with uniforms before finding out associated block?
}

void Material::get_user_block(std::string user_block) {
	block_index = glGetUniformBlockIndex(shader->id, user_block.c_str());
	if (block_index != GL_INVALID_INDEX) {
		glGetActiveUniformBlockiv(shader->id, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
		block_buffer = (GLubyte*)std::malloc(block_size);

		GLint uniform_count = 0;
		glGetActiveUniformBlockiv(shader->id, block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniform_count);

		GLuint* indices = new GLuint[uniform_count];
		GLint* offsets = new GLint[uniform_count];
		GLint* types = new GLint[uniform_count];
		GLint* sizes = new GLint[uniform_count];

		glGetActiveUniformBlockiv(shader->id, block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)indices);
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
			block_uniforms[buffer] = new_uniform;
		}
		delete[] indices;
		delete[] offsets;
		delete[] types;
		delete[] sizes;

		//@TODO Offload UBO to Shader object? Only update buffer from Material?
		glGenBuffers(1, &block_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, block_ubo);
		glBufferData(GL_UNIFORM_BUFFER, block_size, block_buffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, block_index, block_ubo);
	}
}

void Material::get_default_block() {
	GLint uniform_count = 0;
	glGetProgramiv(shader->id, GL_ACTIVE_UNIFORMS, &uniform_count);
	GLuint* indices = new GLuint[uniform_count];
	GLint* offsets = new GLint[uniform_count];
	GLint* types = new GLint[uniform_count];
	GLint* sizes = new GLint[uniform_count];

	for (uint i = 0; i < uniform_count; i++) {
		GLchar buffer[256];
	}

	delete[] indices;
	delete[] offsets;
	delete[] types;
	delete[] sizes;
}

Shader* Material::get_shader() {
	return shader;
}

void Material::set_matrix(std::string name, glm::mat4 value) {
	auto it = block_uniforms.find(name);
	if (it != block_uniforms.end()) {
		uniform_meta meta = it->second;
		//assert(meta.size == sizeof(glm::mat4));
		std::memcpy(block_buffer + meta.offset, glm::value_ptr(value), sizeof(glm::mat4));
	}
	else {
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniformMatrix4fv(index, 1, false, glm::value_ptr(value));
	}
	update_buffer = true;
}

void Material::set_vec4(std::string name, glm::vec4 value) {
	auto it = block_uniforms.find(name);
	if (it != block_uniforms.end()) {
		uniform_meta meta = it->second;
		assert(meta.size == sizeof(glm::vec4));
		std::memcpy(block_buffer + meta.offset, glm::value_ptr(value), sizeof(glm::vec4));
	}
	else {
		glUseProgram(shader->id);
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniform4fv(index, 1, glm::value_ptr(value));
	}
	update_buffer = true;
}
void Material::set_vec4(std::string name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	GLfloat tmp[4] = { x, y, z, w };
	set_floatv(name, tmp, 4);
}

void Material::set_vec3(std::string name, glm::vec3 value) {
	auto it = block_uniforms.find(name);
	if (it != block_uniforms.end()) {
		uniform_meta meta = it->second;
		//assert(meta.size == sizeof(glm::vec4));
		std::memcpy(block_buffer + meta.offset, glm::value_ptr(value), sizeof(glm::vec3));
	}
	else {
		glUseProgram(shader->id);
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniform3fv(index, 1, glm::value_ptr(value));
	}
	update_buffer = true;
}
void Material::set_vec3(std::string name, GLfloat x, GLfloat y, GLfloat z) {
	GLfloat tmp[3] = { x, y, z };
	set_floatv(name, tmp, 3);
}

void Material::set_float(std::string name, GLfloat value) {
	auto it = block_uniforms.find(name);
	if (it != block_uniforms.end()) {
		uniform_meta meta = it->second;
		//assert(meta.size == sizeof(GLfloat));
		std::memcpy(block_buffer + meta.offset, &value, sizeof(GLfloat));
	}
	else {
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniform1f(index, value);
	}
	update_buffer = true;
}

void Material::set_floatv(std::string name, GLfloat* values, GLuint size) {
	auto it = block_uniforms.find(name);
	if (it != block_uniforms.end()) {
		uniform_meta meta = it->second;
		//assert(meta.size == sizeof(GLfloat));
		std::memcpy(block_buffer + meta.offset, values, size * sizeof(GLfloat));
	}
	else {
		GLint index = glGetUniformLocation(shader->id, name.c_str());
		glUniform1fv(index, size, values);
	}
	update_buffer = true;
}

void Material::bind() {
	//@SPEED Get buffer block binding point, bind program AND stream buffer data to
	// GPU. In future, group renderable objects by shader program so that
	// buffer binding point remains the same throughout all material instance binds
	// so that only the data needs to be streamed.

	//@SPEED glBufferData, glBufferSubData, glMapBuffer, glBindBufferRange.
	// call glBindBufferSubData for every uniform update?

	//@SPEED send important matrices through attributes rather than shaders?
	if (shader != nullptr) {
		glUseProgram(shader->id);
		glBindBufferBase(GL_UNIFORM_BUFFER, block_index, block_ubo);
		if (update_buffer) {
			glBindBuffer(GL_UNIFORM_BLOCK, block_ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, block_size, block_buffer);
			update_buffer = false;
		}
	}
}

