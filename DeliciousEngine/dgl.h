#ifndef DELICIOUS_DGL_H
#define DELICIOUS_DGL_H

#include <GL/glew.h>
#include <string>

namespace dgl {

	bool compile(const GLuint& object, const std::string& source);

}

#endif