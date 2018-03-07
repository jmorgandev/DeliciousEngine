#include "mesh_renderer.h"
#include <glew.h>
#include "material.h"
#include "mesh.h"

MeshRenderer::MeshRenderer() {
	mesh = nullptr;
	material = nullptr;
	visible = true;
}

void MeshRenderer::draw() {
	glEnable(GL_DEPTH_TEST);
	if (mesh != nullptr && material->get_shader() != nullptr && visible) {
		material->bind();
		glBindVertexArray(mesh->vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
	}
}