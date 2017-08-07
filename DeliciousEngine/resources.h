#ifndef DELICIOUS_RESOURCES_H
#define DELICIOUS_RESOURCES_H

#include <string>
#include <unordered_map>

#include "dtypes.h"
#include "mesh_data.h"

#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "font.h"

class Engine;
class Console;

class Resources {
public:
	bool init(Engine* engine_in);
	void cleanup();

	Texture* load_texture(std::string filepath);
	Shader* load_shader(std::string filepath);

	Texture* fetch_texture(std::string filename);
	Shader* fetch_shader(std::string filename);

	Font* make_font(std::string name, Texture* texture_in, Shader* shader_in);
	
	Mesh* fetch_mesh(std::string filename);
	Mesh* make_mesh(std::string name, float* data);	
private:
	Console* console_ref;

	typedef std::unordered_map<std::string, Texture>::value_type texture_keypair;
	std::unordered_map<std::string, Texture> texture_catalog;

	typedef std::unordered_map<std::string, Shader>::value_type shader_keypair;
	std::unordered_map<std::string, Shader> shader_catalog;

	typedef std::unordered_map<std::string, Font>::value_type font_keypair;
	std::unordered_map<std::string, Font> font_catalog;

	typedef std::unordered_map<std::string, Mesh>::value_type mesh_keypair;
	std::unordered_map<std::string, Mesh> mesh_catalog;
};

#endif