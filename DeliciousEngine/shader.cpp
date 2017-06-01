#include "shader.h"

Shader::Shader(GLuint object_in) {
	gpu_object = object_in;
}
Shader::~Shader() {
	glDeleteProgram(gpu_object);
}

void Shader::bind() {
	
}

GLuint Shader::get_gpu_object() {
	return gpu_object;
}