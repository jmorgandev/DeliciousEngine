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
	IMG_Init(IMG_INIT_PNG);

	make_mesh("triangle", std_triangle);
	make_mesh("quad", std_quad);
	//make_mesh("quad", primitive_quad);

	return true;
}

void Resources::cleanup() {
	
}

Texture* Resources::load_texture(std::string filepath) {
	Texture new_texture = {};

	SDL_Surface* temp_surface = IMG_Load(filepath.c_str());

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
	glTextureStorage2D(texture_object, 1, GL_RGB8, temp_surface->w, temp_surface->h);
	glTextureSubImage2D(texture_object, 0, 0, 0, temp_surface->w, temp_surface->h, GL_RGB, GL_UNSIGNED_BYTE, (byte*)temp_surface->pixels);

	new_texture.gpu_id = texture_object;
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
		return nullptr;
	}
	if (!dgl::compile(fragment_shader, version_string + fragment_source)) {
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

	new_shader.gpu_id = shader_program;
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

Font* Resources::make_font(std::string name, Texture* texture, Shader* shader) {
	font_catalog[name] = { texture, shader };
	return &font_catalog[name];
}

void Resources::load_gui_resources() {
	glCreateVertexArrays(1, &gui_vertex_array);
	glCreateBuffers(1, &gui_vertex_buffer);

	glNamedBufferStorage(gui_vertex_buffer, sizeof(gui_vertices), gui_vertices, NULL);
	glVertexArrayVertexBuffer(gui_vertex_array, 0, gui_vertex_buffer, 0, sizeof(float));
	glVertexArrayAttribFormat(gui_vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(gui_vertex_array, 0);
}

void Resources::unload_gui_resources() {

}