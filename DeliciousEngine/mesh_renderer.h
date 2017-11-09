#ifndef DELICIOUS_MESH_RENDERER_H
#define DELICIOUS_MESH_RENDERER_H

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include <glm/mat4x4.hpp>

class MeshRenderer {
public:
	MeshRenderer();

	void draw(glm::mat4 transform, glm::mat4 view, glm::mat4 projection);

	void set(Mesh* new_mesh, Shader* new_shader);
 
	void    set_shader(Shader* new_shader);
	Shader* get_shader();

	void  set_mesh(Mesh* new_mesh);
	Mesh* get_mesh();

	void set_texture(Texture* new_texture);

	void set_visible(bool v);
	bool is_visible();
private:
	//@TODO: Transition to Material class later.
	Mesh*   mesh;
	Shader* shader;
	bool    visible;

	//@TODO: Store texture here for now.
	Texture* texture;
};

#endif