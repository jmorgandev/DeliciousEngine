#ifndef DELICIOUS_SHADER_H
#define DELICIOUS_SHADER_H

#include <glad.h>
#include <string>
#include <unordered_map>

//@Todo: Move material meta data (uniforms, samplers) to here instead.
// -Enabled attributes
// -Uniform list/count
// -Metadata

struct Shader {
	GLuint id;
	std::unordered_map<std::string, GLint> uniform_map;
};

#endif