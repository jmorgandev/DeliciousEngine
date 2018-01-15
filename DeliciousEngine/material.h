#ifndef DELICIOUS_MATERIAL_H
#define DELICIOUS_MATERIAL_H

//Manages the attributes & uniforms of a shader.
#include "shader.h"
#include "texture.h"
#include "dtypes.h"
#include "mesh.h"

#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>

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
	~MaterialX();

	void bind();

	void    set_shader(Shader* value);
	Shader* get_shader();

	void set_matrix(std::string name, glm::mat4 value);
	void set_vector4(std::string name, glm::vec4 value);
	void set_vector3(std::string name, glm::vec3 value);
	void set_floatv(std::string name, GLfloat* values, GLuint size);
	void set_float(std::string name, GLfloat value);
private:
	Shader* shader;
	Texture* textures[MATERIAL_MAX_TEX];

	GLuint   mblock_index;
	GLuint   mblock_ubo;
	GLint    mblock_size;
	GLubyte* mblock_buffer;

	struct uniform_meta {
		GLuint index;
		GLint offset;
		GLint type;
		GLint size;
	};
	std::map<std::string, uniform_meta> uniform_list;
	bool update_buffer;
};

#endif