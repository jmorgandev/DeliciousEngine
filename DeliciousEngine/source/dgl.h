#ifndef DELICIOUS_DGL_H
#define DELICIOUS_DGL_H

#include <glad.h>
#include <string>
#include "mesh_data.h"

namespace dgl {

	bool compile(const GLuint& object, const std::string& source);
	bool compile(const GLuint& object, const GLchar* source);

	bool is_sampler(const GLuint& uniform_type);
}

#endif