#ifndef DELICIOUS_MESH_H
#define DELICIOUS_MESH_H

#include "dtypes.h"
#include <GL/glew.h>

enum MeshAttributes {
	VERTICES,
	NORMALS,
	TEXCOORDS,
	COLORS,
	ATTRIBUTE_COUNT
};
struct Mesh {
	GLuint vao;
	GLuint vbo[ATTRIBUTE_COUNT];
	uint32 vertex_count;
	uint32 triangle_count;
};

#endif