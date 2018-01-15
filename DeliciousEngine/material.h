#ifndef DELICIOUS_MATERIAL_H
#define DELICIOUS_MATERIAL_H

//Manages the attributes & uniforms of a shader.
#include "shader.h"
#include "texture.h"
#include "dtypes.h"
#include "mesh.h"

#include <vector>

//@TODO: Auto-generate global uniform block for standard engine assigned uniforms e.g:
// - matrix_mvp (Computed before sending to GPU)
// - matrix_view
// - matrix_projection
// - matrix_model
// - camera frustrum?
// - global lights? etc...

const uint MATERIAL_MAX_TEX = 8;

struct Material {
	Texture* texture;
	Shader*  shader;
};

class MaterialX {
public:
	MaterialX();
	~MaterialX() {}

	void    set_shader(Shader* value);
	Shader* get_shader();
private:
	Shader* shader;
	Texture* textures[MATERIAL_MAX_TEX];

	GLuint  mblock_index;
	GLuint  mblock_ubo;
	GLint   mblock_size;
	GLvoid* mblock_buffer;
};

#endif