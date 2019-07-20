#ifndef DELICIOUS_RESOURCES_H
#define DELICIOUS_RESOURCES_H

#include <string>
#include <unordered_map>
#include "system.h"

#include "dtypes.h"
#include "mesh_data.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "asset_loader.h"

class Resources : public System {
public:
	bool load() override;
	bool start() override { return load_default_resources(); }
	bool free() override;

	bool load_default_resources();

	Texture* load_texture(std::string filepath);
	Texture* load_texture(std::string filepath, std::string id);
	Shader*  load_shader(std::string filepath);

	Texture* fetch_texture(std::string filename);
	Shader*  fetch_shader(std::string filename);
	
	Mesh* fetch_mesh(std::string filename);
	//@Temp
	Mesh* make_mesh(std::string name, MeshData data);

	Material* fetch_material(std::string filename);
	//@Temp
	Material* make_material(std::string name, Shader* shader);
private:
	std::unordered_map<std::string, Texture>  texture_catalog;
	std::unordered_map<std::string, Shader>   shader_catalog;
	std::unordered_map<std::string, Mesh>     mesh_catalog;
	std::unordered_map<std::string, Material> material_catalog;
};
extern Resources resources;

#endif