#include "material.h"
#include <glm/gtc/type_ptr.hpp>
#include "dgl.h"
#include "shader.h"
#include "texture.h"

Material::Material(Shader* shader_program, std::string user_block) {
	userblock_index = GL_INVALID_INDEX;
	userblock_buffer = nullptr;
	update_buffer = false;
	shader = shader_program;

	identify_uniforms();
	identify_userblock(user_block);
}

Material::~Material() {
	if (userblock_buffer != nullptr) {
		std::free(userblock_buffer);
		userblock_buffer = nullptr;
	}
}

void Material::identify_uniforms() {
	GLint uniform_count = 0;
	glGetProgramInterfaceiv(shader->id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniform_count);
	const GLenum query[4] = { GL_LOCATION, GL_OFFSET, GL_TYPE, GL_BLOCK_INDEX };
	for (uint i = 0; i < uniform_count; i++) {
		GLint data[4];
		glGetProgramResourceiv(shader->id, GL_UNIFORM, i, 4, query, 4, NULL, data);
		GLchar buffer[256];
		glGetProgramResourceName(shader->id, GL_UNIFORM, i, 256, NULL, buffer);

		if (dgl::is_sampler(data[2])) {
			if (sampler_list.size() != MAX_TEXTURES) {
				GLint next_binding = (GLint)sampler_list.size();
				sampler_list[buffer] = { data[0], data[2], next_binding, nullptr };
				glUniform1i(data[0], next_binding);
			}
			else {
				//@TODO: Print some error to the console... Used too many textures...
			}
		}
		else uniform_list[buffer] = { data[0], data[1], data[2], data[3] };
	}
}

void Material::identify_userblock(std::string blockname) {
	userblock_index = glGetProgramResourceIndex(shader->id, GL_UNIFORM_BLOCK, blockname.c_str());
	if (userblock_index != GL_INVALID_INDEX) {
		glGetActiveUniformBlockiv(shader->id, userblock_index, GL_UNIFORM_BLOCK_DATA_SIZE, &userblock_size);
		
		//@MEMORY currently store one buffer in RAM and another on the GPU, just for the sake of slightly quicker
		//		  buffer updating with glBufferSubData / glBufferData. Probably not necessary...
		userblock_buffer = new GLubyte[userblock_size];

		//@TODO Offload UBO to Shader object? Only update buffer from Material?
		glGenBuffers(1, &userblock_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, userblock_ubo);
		glBufferData(GL_UNIFORM_BUFFER, userblock_size, userblock_buffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, userblock_index, userblock_ubo);
	}
	else {
		//@TODO: Print a warning to the console... maybe...
	}
}

Shader* Material::get_shader() {
	return shader;
}

void Material::set_matrix(std::string name, glm::mat4 value) {
	auto it = uniform_list.find(name);
	if (it != uniform_list.end()) {
		const uniform_meta& uniform = it->second;
		if (uniform.block != GL_INVALID_INDEX && uniform.block == userblock_index) {
			memcpy(userblock_buffer + uniform.offset, glm::value_ptr(value), sizeof(glm::mat4));
		}
		else {
			glUniformMatrix4fv(uniform.location, 1, GL_FALSE, glm::value_ptr(value));
		}
		update_buffer = true;
	}
	else {
		//ERROR, attempted to access non-existant/non-active uniform
	}	
}

void Material::set_vec4(std::string name, glm::vec4 value) {
	auto it = uniform_list.find(name);
	if (it != uniform_list.end()) {
		const uniform_meta& uniform = it->second;
		if (uniform.block != GL_INVALID_INDEX && uniform.block == userblock_index) {
			memcpy(userblock_buffer + uniform.offset, glm::value_ptr(value), sizeof(glm::vec4));
		}
		else {
			glUniform4f(uniform.location, value.x, value.y, value.z, value.w);
		}
		update_buffer = true;
	}
	else {
		//ERROR, attempted to access non-existant/non-active uniform
	}
}
void Material::set_vec4(std::string name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	GLfloat tmp[4] = { x, y, z, w };
	set_floatv(name, tmp, 4);
}

void Material::set_vec3(std::string name, glm::vec3 value) {
	auto it = uniform_list.find(name);
	if (it != uniform_list.end()) {
		const uniform_meta& uniform = it->second;
		if (uniform.block != GL_INVALID_INDEX && uniform.block == userblock_index) {
			memcpy(userblock_buffer + uniform.offset, glm::value_ptr(value), sizeof(glm::vec3));
		}
		else {
			glUniform3f(uniform.location, value.x, value.y, value.z);
		}
		update_buffer = true;
	}
	else {
		//ERROR, attempted to access non-existant/non-active uniform
	}
}
void Material::set_vec3(std::string name, GLfloat x, GLfloat y, GLfloat z) {
	GLfloat tmp[3] = { x, y, z };
	set_floatv(name, tmp, 3);
}

void Material::set_float(std::string name, GLfloat value) {
	auto it = uniform_list.find(name);
	if (it != uniform_list.end()) {
		const uniform_meta& uniform = it->second;
		if (uniform.block != GL_INVALID_INDEX && uniform.block == userblock_index) {
			memcpy(userblock_buffer + uniform.offset, &value, sizeof(GLfloat));
		}
		else {
			glUniform1f(uniform.location, value);
		}
		update_buffer = true;
	}
	else {
		//ERROR, attempted to access non-existant/non-active uniform
	}
}

void Material::set_floatv(std::string name, GLfloat* values, GLuint size) {
	auto it = uniform_list.find(name);
	if (it != uniform_list.end()) {
		const uniform_meta& uniform = it->second;
		if (uniform.block != GL_INVALID_INDEX && uniform.block == userblock_index) {
			memcpy(userblock_buffer + uniform.offset, values, sizeof(GLfloat) * size);
		}
		else {
			glUniform1fv(uniform.location, size, values);
		}
		update_buffer = true;
	}
	else {
		//ERROR, attempted to access non-existant/non-active uniform
	}
}

void Material::set_texture(std::string name, Texture* tex) {
	auto it = sampler_list.find(name);
	if (it != sampler_list.end()) {
		it->second.texture = tex;
	}
	else {
		//Non existant sampler uniform
	}
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
		if (userblock_index != GL_INVALID_INDEX) {
			glBindBufferBase(GL_UNIFORM_BUFFER, userblock_index, userblock_ubo);
			if (update_buffer) {
				glBindBuffer(GL_UNIFORM_BLOCK, userblock_ubo);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, userblock_size, userblock_buffer);
				update_buffer = false;
			}
		}
		//Bind appropriate textures
		for (auto item : sampler_list) {
			const sampler_meta& sampler = item.second;
			glActiveTexture(GL_TEXTURE0 + sampler.binding);
			if (item.second.texture == nullptr) glBindTexture(sampler.type, NULL);
			else glBindTexture(GL_TEXTURE_2D, sampler.texture->id);
			//@TODO: Get the texture target from the texture object in future
		}
	}
}

