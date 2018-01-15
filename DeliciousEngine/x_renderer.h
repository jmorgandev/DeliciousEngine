#pragma once

struct Mesh;
class MaterialX;

class MeshRendererX {
public:
	MeshRendererX();

	void draw();

	void set(Mesh* new_mesh, MaterialX* new_material);
private:
	Mesh * mesh;
	MaterialX* material;
	bool      visible;
};