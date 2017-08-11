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

	Font* fetch_font(std::string name);
	Font* make_font(std::string name, Texture* texture_in, Shader* shader_in, int font_px);
	
	Mesh* fetch_mesh(std::string filename);
	Mesh* make_mesh(std::string name, MeshData data);	
private:
	Console* console_ref;

	std::unordered_map<std::string, Texture> texture_catalog;
	std::unordered_map<std::string, Shader> shader_catalog;
	std::unordered_map<std::string, Font> font_catalog;
	std::unordered_map<std::string, Mesh> mesh_catalog;

	void load_gui_resources();
	void unload_gui_resources();
	GLuint gui_vertex_array;
	GLuint gui_vertex_buffers[2];
};

#endif