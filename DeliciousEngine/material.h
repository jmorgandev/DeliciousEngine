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

class Material {
public:
	Material(Shader* shader_program = nullptr, std::string user_block = "material");
	~Material();

	void bind();

	Shader* get_shader();

	void set_matrix(std::string name, glm::mat4 value);

	void set_vec4(std::string name, glm::vec4 value);
	void set_vec4(std::string name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

	void set_vec3(std::string name, glm::vec3 value);
	void set_vec3(std::string name, GLfloat x, GLfloat y, GLfloat z);

	void set_float(std::string name, GLfloat value);
	void set_floatv(std::string name, GLfloat* values, GLuint size);
private:
	Shader* shader;

	static const uint MAX_TEXTURES = 16;	//Support GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS ?
	GLenum sampler2d[MAX_TEXTURES];
	uint sampler2d_count;

	GLuint   block_index;
	GLuint   block_ubo;
	GLint    block_size;
	GLubyte* block_buffer;

	struct uniform_meta {
		GLuint index;
		GLint offset;
		GLint type;
		GLint size;
	};
	std::map<std::string, uniform_meta> block_uniforms;
	bool update_buffer;

	std::map<std::string, GLuint> uniforms;

	void get_user_block(std::string name);
	void get_default_block();

	void get_block(std::string name);
};

#endif