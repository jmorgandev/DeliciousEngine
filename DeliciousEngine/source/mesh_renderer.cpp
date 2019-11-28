#include "mesh_renderer.h"
#include <glad.h>
#include "material.h"
#include "mesh.h"

MeshRenderer::MeshRenderer() {
	mesh = nullptr;
	material = nullptr;
	visible = true;
}

void MeshRenderer::draw() {
	glEnable(GL_DEPTH_TEST);
	//if (mesh != nullptr && material->get_shader() != nullptr && visible) {
	//	material->bind();
	//	glBindVertexArray(mesh->vao);
	//	glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
	//}

	if (mesh != nullptr && visible)	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->texture);
		material->bind();
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
	}
}