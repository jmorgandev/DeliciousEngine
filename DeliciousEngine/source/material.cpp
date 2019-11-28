#include "material.h"

#include <glad.h>
#include <gtc/type_ptr.hpp>

#include "shader.h"
#include "texture.h"

Material::Material(Shader* program) {
	shader = program;
}

void Material::bind() {
	glUseProgram(shader->id);
}

void Material::set_matrix(std::string name, glm::mat4 m) {
	int loc = glGetUniformLocation(shader->id, name.c_str());
	if (loc != -1) {
		bind();
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m));
	}
}

void Material::set_vec4(std::string name, glm::vec4 v) {
	int loc = glGetUniformLocation(shader->id, name.c_str());
	if (loc != -1) {
		bind();
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	}
}

void Material::set_vec3(std::string name, glm::vec3 v) {
	int loc = glGetUniformLocation(shader->id, name.c_str());
	if (loc != -1) {
		bind();
		glUniform3f(loc, v.x, v.y, v.z);
	}
}

void Material::set_float(std::string name, float f) {
	int loc = glGetUniformLocation(shader->id, name.c_str());
	if (loc != -1) {
		bind();
		glUniform1f(loc, f);
	}
}

void Material::set_texture(std::string name, const Texture* tex) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->id);
}