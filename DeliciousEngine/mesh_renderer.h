#ifndef DELICIOUS_MESH_RENDERER_H
#define DELICIOUS_MESH_RENDERER_H

#include "mesh.h"
#include "material.h"
#include <glm/mat4x4.hpp>

class MeshRenderer {
public:
	MeshRenderer();

	void draw(glm::mat4 transform, glm::mat4 view, glm::mat4 projection);

	void set(Mesh* new_mesh, Material* new_material);

	void  set_mesh(Mesh* new_mesh);
	Mesh* get_mesh();

	void      set_material(Material* new_material);
	Material* get_material();

	void set_visible(bool v);
	bool is_visible();
private:
	Mesh*     mesh;
	Material* material;
	bool      visible;
};

#endif