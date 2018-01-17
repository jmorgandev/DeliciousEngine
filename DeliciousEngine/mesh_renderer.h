#ifndef DELICIOUS_MESH_RENDERER_H
#define DELICIOUS_MESH_RENDERER_H

class Material;
struct Mesh;

class MeshRenderer {
public:
	MeshRenderer();

	void draw();

	void set(Mesh* new_mesh, Material* new_material);
private:
	Mesh*     mesh;
	Material* material;
	bool      visible;
};

#endif