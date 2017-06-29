#include "mesh.h"

Mesh::Mesh(GLuint vao, GLuint* vbos, uint32 v_count, uint32 t_count) {
	vertex_array = vao;
	for (int i = 0; i < BUFFER_COUNT; i++) {
		vertex_buffers[i] = vbos[i];
	}
	vertex_count = v_count;
	triangle_count = t_count;
}

GLuint Mesh::get_vertex_array() {
	return vertex_array;
}

GLuint Mesh::get_vertex_buffer(int index) {
	return vertex_buffers[index];
}

uint32 Mesh::get_vertex_count() {
	return vertex_count;
}

uint32 Mesh::get_triangle_count() {
	return triangle_count;
}