#ifndef DELICIOUS_SHADER_H
#define DELICIOUS_SHADER_H

#include <glad.h>

//@Todo: Move material meta data (uniforms, samplers) to here instead.
// -Enabled attributes
// -Uniform list/count
// -Metadata

struct Shader {
	GLuint id;

	GLint attribute_count;
	GLint uniform_count;
};

#endif