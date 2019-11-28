#ifndef DELICIOUS_MESH_H
#define DELICIOUS_MESH_H

#include <glad.h>

#include "dtypes.h"

struct Mesh {
	GLuint vbo;
	int stride;
	uint texture;
	uint32 vertex_count;
};

#endif