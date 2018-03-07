#ifndef DELICIOUS_MESH_RENDERER_H
#define DELICIOUS_MESH_RENDERER_H

#include "mesh.h"
#include "material.h"

class MeshRenderer {
public:
	MeshRenderer();

	void draw();

	MeshRenderer& set_mesh(Mesh* const mesh) { this->mesh = mesh; return *this; }
	MeshRenderer& set_material(Material* const material) { this->material = material; return *this; }

	Mesh* get_mesh() const { return mesh; }
	Material* get_material() const { return material; }

	void display(const bool& is_visible) { visible = is_visible; }
	bool is_visible() const { return visible; }

private:
	Mesh*     mesh;
	Material* material;
	bool      visible;
};

#endif