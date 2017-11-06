#include "mesh_renderer.h"

MeshRenderer::MeshRenderer() {
	mesh = nullptr;
	shader = nullptr;
}

void MeshRenderer::draw() {
	if (mesh != nullptr && shader != nullptr && visible == true) {
		//@TODO - Add render code here
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