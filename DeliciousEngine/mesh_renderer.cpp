#include "mesh_renderer.h"

MeshRenderer::MeshRenderer() {
	mesh = nullptr;
	material = nullptr;
	visible = true;
}

void MeshRenderer::draw(glm::mat4 transform, glm::mat4 view, glm::mat4 projection) {
	glEnable(GL_DEPTH_TEST);
	if (mesh != nullptr && material->shader != nullptr && visible == true) {
		GLuint id = material->shader->id;
		int transform_uniform  = glGetUniformLocation(id, "transform");
		int view_uniform       = glGetUniformLocation(id, "view");
		int projection_uniform = glGetUniformLocation(id, "projection");

		glUseProgram(id);
		if (material->texture != nullptr) {
			glBindTexture(GL_TEXTURE_2D, material->texture->id);
		}
		glUniformMatrix4fv(transform_uniform,  1, false, &transform[0][0]);
		glUniformMatrix4fv(view_uniform,	   1, false, &view[0][0]);
		glUniformMatrix4fv(projection_uniform, 1, false, &projection[0][0]);

		glBindVertexArray(mesh->vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
	}
}

void MeshRenderer::set(Mesh* new_mesh, Material* new_material) {
	mesh = new_mesh;
	material = new_material;
}

void MeshRenderer::set_mesh(Mesh* new_mesh) {
	mesh = new_mesh;
}
Mesh* MeshRenderer::get_mesh() {
	return mesh;
}

void MeshRenderer::set_material(Material* new_material) {
	material = new_material;
}
Material* MeshRenderer::get_material() {
	return material;
}

void MeshRenderer::set_visible(bool v) {
	visible = v;
}
bool MeshRenderer::is_visible() {
	return visible;
}