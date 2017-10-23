#include "resources.h"

#include <GL/glew.h>
#include <SDL/SDL_image.h>
#include <algorithm>
#include <glm/geometric.hpp>
#include "console.h"
#include "screen.h"
#include "dff.h"
#include "dcf.h"
#include "dgl.h"
#include "primitives.h"
#include "build_info.h"
#include <iostream>

bool Resources::init(System_Ref sys) {
	system = sys;

	IMG_Init(IMG_INIT_PNG | IMG_INIT_TIF);

	return true;
}

void Resources::cleanup() {
	unload_gui_resources();
}

Texture* Resources::load_texture(std::string filepath) {
	Texture new_texture = {};

	SDL_Surface* temp_surface = IMG_Load(filepath.c_str());
	if (temp_surface == nullptr) {
		//Error, extension not supported by SDL_Image
		return nullptr;
	}
	//Otherwise we have the surface
	int bpp = temp_surface->format->BytesPerPixel;

	GLuint texture_object;
	glGenTextures(1, &texture_object);
	glBindTexture(GL_TEXTURE_2D, texture_object);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, temp_surface->w, temp_surface->h);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, temp_surface->w, temp_surface->h, GL_RGBA, GL_UNSIGNED_BYTE, (byte*)temp_surface->pixels);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//@TODO - Have some way of specifying the texture parameters outside this function

	new_texture.id = texture_object;
	new_texture.width = temp_surface->w;
	new_texture.height = temp_surface->h;
	new_texture.bytes_per_pixel = temp_surface->format->BytesPerPixel;

	SDL_FreeSurface(temp_surface);

	texture_catalog[filepath] = new_texture;
	return &texture_catalog[filepath];
}
Texture* Resources::fetch_texture(std::string filename) {
	auto it = texture_catalog.find(filename);
	if (it == texture_catalog.end()) {
		return nullptr;
	}
	else {
		return &it->second;
	}
}

Shader* Resources::load_shader(std::string filepath) {
	Shader new_shader = {};

	std::string shader_source = dff::file_str(filepath);
	if (shader_source.empty()) {
		return nullptr;
	}
	std::string vertex_source = dff::get_glsl_region(shader_source, "#scope ", "vertex");
	std::string fragment_source = dff::get_glsl_region(shader_source, "#scope ", "fragment");
	if (vertex_source.empty() || fragment_source.empty()) {
		return nullptr;
	}
	size_t version_index = shader_source.find("#version");
	std::string version_string = "";
	if (version_index != std::string::npos) {
		size_t size = shader_source.find('\n', version_index) - version_index;
		version_string = shader_source.substr(version_index, size);
	}
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!vertex_shader || !fragment_shader) {
		return nullptr;
	}
	if (!dgl::compile(vertex_shader, version_string + vertex_source)) {
		GLchar buffer[1024];
		glGetShaderInfoLog(vertex_shader, 1024, NULL, buffer);
		std::cout << buffer << "\n";
		return nullptr;
	}
	if (!dgl::compile(fragment_shader, version_string + fragment_source)) {
		GLchar buffer[1024];
		glGetShaderInfoLog(fragment_shader, 1024, NULL, buffer);
		std::cout << buffer << "\n";
		return nullptr;
	}
	GLuint shader_program = glCreateProgram();
	if (!shader_program) {
		return nullptr;
	}
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	new_shader.id = shader_program;
	shader_catalog[filepath] = new_shader;
	return &shader_catalog[filepath];
}

Shader* Resources::fetch_shader(std::string filename) {
	auto it = shader_catalog.find(filename);
	if (it == shader_catalog.end()) {
		return nullptr;
	}
	else {
		return &it->second;
	}
}

Mesh * Resources::fetch_mesh(std::string filename) {
	auto it = mesh_catalog.find(filename);
	if (it == mesh_catalog.end()) {
		return nullptr;
	}
	else {
		return &it->second;
	}
}

Mesh* Resources::make_mesh(std::string name, MeshData data) {
	Mesh new_mesh = {};

	//Create VAO
	glGenVertexArrays(1, &new_mesh.vao);
	glBindVertexArray(new_mesh.vao);

	//Create VBOs
	glGenBuffers(NUM_BUFFERS, new_mesh.vbo);

	// VERTICES
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh.vbo[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(glm::vec3), &data.vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// NORMALS
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh.vbo[NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, data.normals.size() * sizeof(glm::vec3), &data.normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	
	// TEXCOORDS
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh.vbo[TEXCOORDS]);
	glBufferData(GL_ARRAY_BUFFER, data.texcoords.size() * sizeof(glm::vec2), &data.texcoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	new_mesh.vertex_count = data.vertices.size();
	new_mesh.triangle_count = new_mesh.vertex_count / 3;
	
	mesh_catalog[name] = new_mesh;
	return &mesh_catalog[name];
}

Font* Resources::make_font(std::string name, Texture* texture, Shader* shader, int font_px) {
	Font new_font = {};

	new_font.char_offset = '!';
	new_font.total_cells = ('~' - '!') + 1;
	new_font.cell_height = font_px;
	new_font.cell_width = (font_px / 2);
	new_font.cell_rows = texture->height / new_font.cell_height;
	new_font.cell_columns = texture->width / new_font.cell_width;

	new_font.gui_vao = gui_vertex_array;
	new_font.shader = shader;
	new_font.texture = texture;

	font_catalog[name] = new_font;
	return &font_catalog[name];
}

Font* Resources::fetch_font(std::string name) {
	auto it = font_catalog.find(name);
	if (it == font_catalog.end()) {
		return nullptr;
	}
	else {
		return &it->second;
	}
}

void Resources::load_gui_resources() {
	glGenVertexArrays(1, &gui_vertex_array);
	glBindVertexArray(gui_vertex_array);

	glGenBuffers(2, gui_vertex_buffers);

	glBindBuffer(GL_ARRAY_BUFFER, gui_vertex_buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gui_vertices), gui_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, gui_vertex_buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gui_texcoords), gui_texcoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
}

void Resources::unload_gui_resources() {
	glDeleteBuffers(2, gui_vertex_buffers);
	glDeleteVertexArrays(1, &gui_vertex_array);
}

bool Resources::load_default_resources() {
	load_gui_resources();

	Shader* font_shader = load_shader("res/bmp_font.glsl");
	if (font_shader == nullptr)	return false;

	Texture* font_texture = load_texture("res/consolas_24.tga");
	if (font_texture == nullptr) return false;

	Font* con_font = make_font("consolas", font_texture, font_shader, 24);
	if (con_font == nullptr) return false;
	system.console->set_font(con_font);

	Shader* box_shader = load_shader("res/shape.glsl");
	if (box_shader == nullptr) return false;
	system.console->set_gui_properties(gui_vertex_array, box_shader);

	return true;
}