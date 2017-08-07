#ifndef DELICIOUS_DGL_H
#define DELICIOUS_DGL_H

#include <GL/glew.h>
#include <string>
#include "mesh_data.h"

namespace dgl {

	bool compile(const GLuint& object, const std::string& source);
	bool compile(const GLuint& object, const GLchar* source);

	GLuint create_vao();

	//int validate_meshdata(MeshData& data);
	
}

#endif