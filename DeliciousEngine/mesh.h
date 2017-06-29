#ifndef DELICIOUS_MESH_H
#define DELICIOUS_MESH_H

#include "dtypes.h"
#include <GL/glew.h>

enum MeshAttribute {
	VERTEX_BUFFER,
	NORMAL_BUFFER,
	COLOR_BUFFER,
	TEXCOORD_BUFFER,
	INDEX_BUFFER,
	BUFFER_COUNT
};
class Mesh {
public:
	Mesh(GLuint vao, GLuint* vbos, uint32 v_count, uint32 t_count);
	GLuint get_vertex_array();
	GLuint get_vertex_buffer(int index);
	uint32 get_vertex_count();
	uint32 get_triangle_count();
private:
	GLuint vertex_array;
	GLuint vertex_buffers[BUFFER_COUNT];
	uint32 vertex_count;
	uint32 triangle_count;
};

#endif