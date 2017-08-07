#ifndef DELICIOUS_MESH_H
#define DELICIOUS_MESH_H

#include "dtypes.h"
#include <GL/glew.h>

enum MeshAttributes {
	VERTICES,
	NORMALS,
	TEXCOORDS,
	NUM_BUFFERS
};
struct Mesh {
	GLuint vao;
	GLuint vbo[NUM_BUFFERS];
	uint32 vertex_count;
	uint32 triangle_count;
};

#endif