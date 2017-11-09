#include "mesh_renderer.h"

MeshRenderer::MeshRenderer() {
	mesh    = nullptr;
	shader  = nullptr;
	visible = true;
	texture = nullptr;
}

void MeshRenderer::draw(glm::mat4 transform, glm::mat4 view, glm::mat4 projection) {
	glEnable(GL_DEPTH_TEST);
	if (mesh != nullptr && shader != nullptr && visible == true) {
		int transform_uniform  = glGetUniformLocation(shader->id, "transform");
		int view_uniform       = glGetUniformLocation(shader->id, "view");
		int projection_uniform = glGetUniformLocation(shader->id, "projection");

		glUseProgram(shader->id);
		if (texture != nullptr) {
			glBindTexture(GL_TEXTURE_2D, texture->id);
		}
		glBindVertexArray(mesh->vao);
		glUniformMatrix4fv(transform_uniform, 1, false, &transform[0][0]);
		glUniformMatrix4fv(view_uniform, 1, false, &view[0][0]);
		glUniformMatrix4fv(projection_uniform, 1, false, &projection[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
	}
}

void MeshRenderer::set(Mesh* new_mesh, Shader* new_shader) {
	mesh = new_mesh;
	shader = new_shader;
}

void MeshRenderer::set_shader(Shader* new_shader) {
	shader = new_shader;
}
Shader* MeshRenderer::get_shader() {
	return shader;
}

void MeshRenderer::set_mesh(Mesh* new_mesh) {
	mesh = new_mesh;
}
Mesh* MeshRenderer::get_mesh() {
	return mesh;
}

void MeshRenderer::set_visible(bool v) {
	visible = v;
}
bool MeshRenderer::is_visible() {
	return visible;
}

void MeshRenderer::set_texture(Texture* new_texture) {
	texture = new_texture;
}