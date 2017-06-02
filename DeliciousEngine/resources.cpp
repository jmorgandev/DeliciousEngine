#include "resources.h"

#include <GL/glew.h>

#include <SDL/SDL_image.h>
#include "engine.h"
#include "console.h"
#include "dff.h"
#include "dcf.h"
#include "dgl.h"

#include "texture.h"
#include "shader.h"

bool Resources::init(Engine* engine_in) {
	console_ref = engine_in->get_console();
	return true;
}

void Resources::load_texture(std::string filepath) {

	SDL_Surface* temp_surface = IMG_Load(filepath.c_str());

	if (temp_surface == NULL) {
		//Error, extension not supported by SDL_Image
		return;
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

	texture_catalog.emplace_back(
		new_object,
		temp_surface->w,
		temp_surface->h,
		32
	);
}

void Resources::load_shader(std::string filepath) {
	std::string filename = dff::path_filename(filepath, false);

	std::string vert_src = dff::file_str(filename + ".vert");
	std::string frag_src = dff::file_str(filename + ".frag");

	if (vert_src == "" || frag_src == "") {
		//ERROR
		return;
	}

	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	if (!vert_shader || !frag_shader) {
		//ERROR
		return;
	}
	if (!dgl::compile(vert_shader, vert_src)) {
		//ERROR
		return;
	}
	if (!dgl::compile(frag_shader, frag_src)) {
		//ERROR
		return;
	}

	GLuint new_program = glCreateProgram();
	if (!new_program) {
		//ERROR
		return;
	}
	glAttachShader(new_program, vert_shader);
	glAttachShader(new_program, frag_shader);
	glLinkProgram(new_program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	shader_catalog.emplace_back(new_program);
}