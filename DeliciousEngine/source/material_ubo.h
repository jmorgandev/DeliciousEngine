#ifndef DELICIOUS_MATERIAL_UBO_H
#define DELICIOUS_MATERIAL_UBO_H

#include <map>
#include <string>

#include <glad.h>
#include <vec3.hpp>
#include <mat4x4.hpp>

#include "dtypes.h"
#include "shader.h"
#include "texture.h"

//@Todo: Auto-generate global uniform block for standard engine assigned uniforms e.g:
// - matrix_mvp (Computed before sending to GPU)
// - matrix_view
// - matrix_projection
// - matrix_model
// - camera frustrum?
// - global lights? etc...

class MaterialUBO {
public:
	MaterialUBO(Shader* shader_program = nullptr, std::string user_block = "material");
	~MaterialUBO();

	void bind();

	Shader* get_shader();

	void set_matrix(std::string name, glm::mat4 value);

	void set_vec4(std::string name, glm::vec4 value);
	void set_vec4(std::string name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

	void set_vec3(std::string name, glm::vec3 value);
	void set_vec3(std::string name, GLfloat x, GLfloat y, GLfloat z);

	void set_float(std::string name, GLfloat value);
	void set_floatv(std::string name, GLfloat* values, GLuint size);

	void set_texture(std::string name, const Texture* tex);

	void set(std::string name, glm::vec4 value);
	void set(std::string name, const Texture* tex);
private:
	Shader* shader;

	static const uint MAX_TEXTURES = 16;	//Support GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS ?

	GLuint   userblock_index;
	GLuint   userblock_ubo;
	GLint    userblock_size;
	GLubyte* userblock_buffer;

	struct UniformMeta {
		GLint location;
		GLint offset;
		GLint type;
		GLint block;
	};
	struct SamplerMeta {
		GLint location;
		GLint type;
		GLint binding;
		const Texture* texture;
	};

	std::map<std::string, UniformMeta> uniform_list;
	std::map<std::string, SamplerMeta> sampler_list;

	bool update_buffer;

	void identify_uniforms();
	void identify_userblock(std::string blockname);
};

#endif