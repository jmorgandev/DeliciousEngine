#include "mesh_renderer.h"

MeshRenderer::MeshRenderer() {
	mesh = nullptr;
	shader = nullptr;
}

void MeshRenderer::draw(glm::mat4 transform, glm::mat4 view, glm::mat4 projection) {
	if (mesh != nullptr && shader != nullptr && visible == true) {
		int transform_uniform = glGetUniformLocation(shader->id, "transform");
		int view_uniform = glGetUniformLocation(shader->id, "view");
		int projection_uniform = glGetUniformLocation(shader->id, "projection");

		glUseProgram(shader->id);
		glBindVertexArray(mesh->vao);
		glUniformMatrix4fv(transform_uniform, 1, false, &transform[0][0]);
		glUniformMatrix4fv(view_uniform, 1, false, &view[0][0]);
		glUniformMatrix4fv(projection_uniform, 1, false, &projection[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
	}
}

void MeshRenderer::set(Mesh* mesh, Shader* shader, bool visible) {
	this->mesh = mesh;
	this->shader = shader;
	this->visible = visible;
}

void MeshRenderer::set_shader(Shader* shader) {
	this->shader = shader;
}
Shader* MeshRenderer::get_shader() {
	return shader;
}

void MeshRenderer::set_mesh(Mesh* mesh) {
	this->mesh = mesh;
}
Mesh* MeshRenderer::get_mesh() {
	return mesh;
}

void MeshRenderer::set_visible(bool visible) {
	this->visible = visible;
}
bool MeshRenderer::is_visible() {
	return visible;
}