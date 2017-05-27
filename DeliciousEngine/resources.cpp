#include "resources.h"

#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include "engine.h"
#include "console.h"
#include "dff.h"
#include "dcf.h"

bool Resources::init(Engine* engine_in) {
	console_ref = engine_in->get_console();
	return true;
}

void Resources::load_texture(cstring filepath) {

	SDL_Surface* temp_surface = IMG_Load(filepath);

	if (temp_surface == NULL) {
		//Error, extension not supported by SDL
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
}

void Resources::load_shader(cstring filepath) {

}