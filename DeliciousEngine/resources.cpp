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
	//make_mesh("quad", primitive_quad);

	return true;
}

void Resources::cleanup() {
	
}

Texture* Resources::load_texture(std::string filepath) {

	std::string filename = dff::path_filename(filepath, false);

	SDL_Surface* temp_surface = IMG_Load(filepath.c_str());

	if (temp_surface == nullptr) {
		//Error, extension not supported by SDL_Image
		return nullptr;
	}
	//Otherwise we have the surface
	GLuint new_object;
	glCreateTextures(GL_TEXTURE_2D, 1, &new_object);

	glTextureStorage2D(new_object, 1, GL_RGBA32F, temp_surface->w, temp_surface->h);

	glTextureSubImage2D(
		new_object,
		0,
		0, 0,
		temp_surface->w,
		temp_surface->h,
		GL_RGBA,
		GL_FLOAT,
		(byte*)temp_surface->pixels
	);
	SDL_FreeSurface(temp_surface);

	texture_catalog.insert(texture_keypair(filename, Texture(new_object, temp_surface->w, temp_surface->h, 32)));
	return &texture_catalog.find(filename)->second;
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
	std::string filename = dff::path_filename(filepath, false);

	std::string vert_src = dff::file_str(filepath + ".vert");
	std::string frag_src = dff::file_str(filepath + ".frag");

	if (vert_src == "" || frag_src == "") {
		//ERROR
		return nullptr;
	}

	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	if (!vert_shader || !frag_shader) {
		return nullptr;
	}
	if (!dgl::compile(vert_shader, vert_src)) {
		GLchar error[1024];
		GLint length;
		glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &length);
		glGetShaderInfoLog(vert_shader, 1024, &length, error);
		std::cout << error << std::endl;
		return nullptr;
	}
	if (!dgl::compile(frag_shader, frag_src)) {
		GLchar error[1024];
		GLint length;
		glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &length);
		glGetShaderInfoLog(frag_shader, 1024, &length, error);
		std::cout << error << std::endl;
		return nullptr;
	}

	GLuint new_program = glCreateProgram();
	if (!new_program) {
		//ERROR
		return nullptr;
	}
	glAttachShader(new_program, vert_shader);
	glAttachShader(new_program, frag_shader);
	glLinkProgram(new_program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	/*std::string filename = dff::path_filename(filepath, false);

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex_shader);
	glAttachShader(shaderProgram, fragment_shader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glUseProgram(shaderProgram);*/
	new_shader.program = new_program;

	shader_catalog.insert(shader_keypair(filename, new_shader));
	return &shader_catalog.find(filename)->second;
}
Shader* Resources::load_glsl(std::string filepath) {
	Shader new_shader = {};

	std::string shader_source = dff::file_str(filepath);
	if (shader_source.empty()) {
		return nullptr;
	}
	std::string vertex_source = dff::get_gls_region(shader_source, "vertex");
	std::string fragment_source = dff::get_gls_region(shader_source, "fragment");
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

	new_shader.program = shader_program;
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
	glCreateBuffers(1, new_mesh.vbo);

	// VERTICES
	glNamedBufferData(new_mesh.vbo[VERTICES], data.vertices.size() * sizeof(glm::vec3), &data.vertices[0], GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(new_mesh.vao, 0, new_mesh.vbo[VERTICES], 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(new_mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(new_mesh.vao, 0);

	//// NORMALS
	//glNamedBufferData(new_mesh.vbo[NORMALS], data.normals.size() * sizeof(glm::vec3), &data.normals[0], GL_STATIC_DRAW);
	//glVertexArrayVertexBuffer(new_mesh.vao, 1, new_mesh.vbo[NORMALS], 0, sizeof(glm::vec3));
	//glVertexArrayAttribFormat(new_mesh.vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
	//glEnableVertexArrayAttrib(new_mesh.vao, 1);
	//
	//// TEXCOORDS
	//glNamedBufferData(new_mesh.vbo[TEXCOORDS], data.texcoords.size() * sizeof(glm::vec2), &data.texcoords[0], GL_STATIC_DRAW);
	//glVertexArrayVertexBuffer(new_mesh.vao, 2, new_mesh.vbo[TEXCOORDS], 0, sizeof(glm::vec2));
	//glVertexArrayAttribFormat(new_mesh.vao, 2, 2, GL_FLOAT, GL_FALSE, 0);
	//glEnableVertexArrayAttrib(new_mesh.vao, 2);
	
	

	mesh_catalog[name] = new_mesh;
	return &mesh_catalog[name];
}

Font* Resources::make_font(std::string name, Texture* texture_in, Shader* shader_in) {
	font_catalog.insert(font_keypair(name, Font(texture_in, shader_in)));
	return &font_catalog.find(name)->second;
}