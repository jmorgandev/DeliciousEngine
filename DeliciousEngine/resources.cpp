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
#include <iostream>

bool Resources::init(Engine* engine_in) {
	console_ref = engine_in->get_console();
	IMG_Init(IMG_INIT_PNG);

	make_mesh("triangle", triangle_shit);
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
		glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &length);
		glGetShaderInfoLog(vert_shader, 1024, &length, error);
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

	glUseProgram(new_program);

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

	shader_catalog.insert(shader_keypair(filename, Shader(new_program)));
	return &shader_catalog.find(filename)->second;
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

Mesh* Resources::make_mesh(std::string name, float* data) {
	Mesh new_mesh = {};
	GLuint vao, vbo;


	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, sizeof(triangle_shit), triangle_shit, GL_STATIC_DRAW);
	glCreateVertexArrays(1, &vao);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 4);
	glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(vao, 0);

	new_mesh.vao = vao;
	new_mesh.vbo = vbo;

	mesh_catalog.insert(mesh_keypair(name, new_mesh));
	return &mesh_catalog.find(name)->second;
}

Font* Resources::make_font(std::string name, Texture* texture_in, Shader* shader_in) {
	font_catalog.insert(font_keypair(name, Font(texture_in, shader_in)));
	return &font_catalog.find(name)->second;
}