#ifndef DELICIOUS_SHADER_H
#define DELICIOUS_SHADER_H

#include <GL/glew.h>

//@TODO: Add more information to shader:
// -Enabled attributes
// -Uniform list/count
// -Metadata

struct Shader {
	GLuint id;

	GLint attribute_count;
	GLint uniform_count;
};

#endif