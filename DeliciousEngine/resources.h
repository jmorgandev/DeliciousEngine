#ifndef DELICIOUS_RESOURCES_H
#define DELICIOUS_RESOURCES_H

#include <string>
#include <unordered_map>

#include "dtypes.h"
#include "mesh_data.h"

#include "shader.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "font.h"

#include "system_ref.h"

class Resources {
public:
	bool init(System_Ref sys);
	void clean_exit();

	bool load_default_resources();

	Texture* load_texture(std::string filepath);
	Shader* load_shader(std::string filepath);

	Texture* fetch_texture(std::string filename);
	Shader* fetch_shader(std::string filename);

	Font* fetch_font(std::string name);
	//@TEMP
	Font* make_font(std::string name, Texture* texture_in, Shader* shader_in, int font_px);
	
	Mesh* fetch_mesh(std::string filename);
	//@TEMP
	Mesh* make_mesh(std::string name, MeshData data);

	Material* fetch_material(std::string filename);
	//@TEMP
	Material* make_material(std::string name, Texture* t, Shader* s);
private:
	System_Ref system;

	std::unordered_map<std::string, Texture>  texture_catalog;
	std::unordered_map<std::string, Shader>   shader_catalog;
	std::unordered_map<std::string, Font>     font_catalog;
	std::unordered_map<std::string, Mesh>     mesh_catalog;
	std::unordered_map<std::string, Material> material_catalog;

	void load_gui_resources();
	void unload_gui_resources();
	void load_mesh_primitives();

	//@TODO: Move GUI related objects to Screen
	GLuint gui_vertex_array;
	GLuint gui_vertex_buffers[2];
};

#endif