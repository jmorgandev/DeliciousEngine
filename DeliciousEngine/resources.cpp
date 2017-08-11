#include "resources.h"

#include <GL/glew.h>
#include <SDL/SDL_image.h>
#include <algorithm>
#include <glm/geometric.hpp>
#include "engine.h"
#include "console.h"
#include "dff.h"
#include "dcf.h"
#include "dgl.h"
#include "std_primitives.h"
#include "build_info.h"
#include <iostream>

bool Resources::init(Engine* engine_in) {
	console_ref = engine_in->get_console();
	IMG_Init(IMG_INIT_PNG | IMG_INIT_TIF);
	

	make_mesh("triangle", std_triangle);
	make_mesh("quad", std_quad);
	//make_mesh("quad", primitive_quad);

	load_gui_resources();

	return true;
}

void Resources::cleanup() {
	unload_gui_resources();
}

Texture* Resources::load_texture(std::string filepath) {
	Texture new_texture = {};

	SDL_Surface* temp_surface = IMG_Load(filepath.c_str());

	int bpp = temp_surface->format->BytesPerPixel;

	if (temp_surface == nullptr) {
		//Error, extension not supported by SDL_Image
		return nullptr;
	}
	//Otherwise we have the surface
	GLuint texture_object;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture_object);

	//glBindTexture(GL_TEXTURE_2D, texture_object);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_surface->w, temp_surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, (byte*)temp_surface->pixels);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTextureStorage2D(texture_object, 1, GL_RGBA32F, temp_surface->w, temp_surface->h);
	glTextureSubImage2D(texture_object, 0, 0, 0, temp_surface->w, temp_surface->h, GL_RGBA, GL_UNSIGNED_BYTE, (byte*)temp_surface->pixels);
	glTextureParameteri(texture_object, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
	std::string version_string = DEFAULT_GLSL_VERSION;
	size_t version_index = shader_source.find("#version");
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

	glCreateVertexArrays(1, &new_mesh.vao);
	glCreateBuffers(NUM_BUFFERS, new_mesh.vbo);

	// VERTICES
	glNamedBufferStorage(new_mesh.vbo[VERTICES], data.vertices.size() * sizeof(glm::vec3), &data.vertices[0], NULL);
	glVertexArrayVertexBuffer(new_mesh.vao, 0, new_mesh.vbo[VERTICES], 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(new_mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(new_mesh.vao, 0);

	// NORMALS
	glNamedBufferStorage(new_mesh.vbo[NORMALS], data.normals.size() * sizeof(glm::vec3), &data.normals[0], NULL);
	glVertexArrayVertexBuffer(new_mesh.vao, 1, new_mesh.vbo[NORMALS], 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(new_mesh.vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(new_mesh.vao, 1);
	
	// TEXCOORDS
	glNamedBufferStorage(new_mesh.vbo[TEXCOORDS], data.texcoords.size() * sizeof(glm::vec2), &data.texcoords[0], NULL);
	glVertexArrayVertexBuffer(new_mesh.vao, 2, new_mesh.vbo[TEXCOORDS], 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(new_mesh.vao, 2, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(new_mesh.vao, 2);

	new_mesh.vertex_count = data.vertices.size();
	new_mesh.triangle_count = new_mesh.vertex_count / 3;
	
	mesh_catalog[name] = new_mesh;
	return &mesh_catalog[name];
}

Font* Resources::make_font(std::string name, Texture* texture, Shader* shader, int font_px) {
	Font new_font = {};

	new_font.char_offset = ' ';
	new_font.total_cells = ('~' - ' ') + 1;
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
	glCreateVertexArrays(1, &gui_vertex_array);
	glCreateBuffers(2, gui_vertex_buffers);

	glNamedBufferStorage(gui_vertex_buffers[0], sizeof(gui_vertices), gui_vertices, NULL);
	glVertexArrayVertexBuffer(gui_vertex_array, 0, gui_vertex_buffers[0], 0, sizeof(float) * 2);
	glVertexArrayAttribFormat(gui_vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(gui_vertex_array, 0);

	glNamedBufferStorage(gui_vertex_buffers[1], sizeof(gui_texcoords), gui_texcoords, NULL);
	glVertexArrayVertexBuffer(gui_vertex_array, 1, gui_vertex_buffers[1], 0, sizeof(float) * 2);
	glVertexArrayAttribFormat(gui_vertex_array, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(gui_vertex_array, 1);
}

void Resources::unload_gui_resources() {
	glDeleteBuffers(2, gui_vertex_buffers);
	glDeleteVertexArrays(1, &gui_vertex_array);
}