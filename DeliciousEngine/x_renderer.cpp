#include "x_renderer.h"
#include <GL/glew.h>
#include "material.h"

MeshRendererX::MeshRendererX() {
	mesh = nullptr;
	material = nullptr;
	visible = true;
}

void MeshRendererX::set(Mesh* new_mesh, MaterialX* new_mat) {
	mesh = new_mesh;
	material = new_mat;
}

void MeshRendererX::draw() {
	glEnable(GL_DEPTH_TEST);
	if (mesh != nullptr && material->get_shader() != nullptr && visible) {
		material->bind();
		glUseProgram(material->get_shader()->id);
		uint c = glGetUniformLocation(material->get_shader()->id, "color");
		glUniform4f(c, 1.0f, 1.0f, 1.0f, 1.0f);
		glBindVertexArray(mesh->vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
	}
}