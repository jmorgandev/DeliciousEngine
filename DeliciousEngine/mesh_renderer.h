#ifndef DELICIOUS_MESH_RENDERER_H
#define DELICIOUS_MESH_RENDERER_H

#include "mesh.h"
#include "shader.h"
#include <glm/mat4x4.hpp>

class MeshRenderer {
public:
	MeshRenderer();

	void draw(glm::mat4 transform, glm::mat4 view, glm::mat4 projection);

	void set(Mesh* mesh, Shader* shader, bool visible = true);

	void    set_shader(Shader* shader);
	Shader* get_shader();

	void  set_mesh(Mesh* mesh);
	Mesh* get_mesh();

	void set_visible(bool visible);
	bool is_visible();
private:
	Mesh*   mesh;
	Shader* shader;
	bool    visible;
};

#endif