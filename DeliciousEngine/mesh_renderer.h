#ifndef DELICIOUS_MESH_RENDERER_H
#define DELICIOUS_MESH_RENDERER_H

#include "mesh.h"
#include "shader.h"

class MeshRenderer {
public:
	MeshRenderer();

	void draw();

	void set(Mesh* mesh, Shader* shader);

	void set_shader(Shader* shader);
	Shader* get_shader();

	void set_mesh(Mesh* mesh);
	Mesh* get_mesh();

	void set_visible(bool v);
	void is_visible();
private:
	Mesh*   mesh;
	Shader* shader;
	bool    visible;
};

#endif