#ifndef DELICIOUS_MATERIAL_H
#define DELICIOUS_MATERIAL_H


#include <string>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include "build_settings.h"

class Texture;
struct Shader;

class Material {
public:
	Material(Shader* program = nullptr);
	~Material() = default;

	void bind();

	void set_matrix(std::string name, glm::mat4 m);
	void set_vec4(std::string name, glm::vec4 v);
	void set_vec3(std::string name, glm::vec3 v);
	void set_float(std::string name, float f);

	void set_texture(std::string name, const Texture* tex);
private:
	Shader* shader;
	Texture* textures[MAX_SHADER_TEXTURES];
};

#endif