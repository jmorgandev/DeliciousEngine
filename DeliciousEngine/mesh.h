#ifndef DELICIOUS_MESH_H
#define DELICIOUS_MESH_H

#include "dtypes.h"
#include <GL/glew.h>

enum MeshAttributes {
	VERTEX_BUFFER,
	NORMAL_BUFFER,
	TEXCOORD_BUFFER,
	BUFFER_COUNT
};
struct Mesh {
	GLuint vao;
	GLuint vbo;
	uint32 vertex_count;
	uint32 triangle_count;
};

#endif