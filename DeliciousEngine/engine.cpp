#include "engine.h"

#include <SDL/SDL_events.h>

bool Engine::init(char** argv, int argc) {
	if (!console.init(this)) {
		return false;
	}
	if (!screen.init(this)) {
		return false;
	}
	
<<<<<<< HEAD
	//resources.load_shader("shader/font");
	//resources.load_texture("texture/con_font.png");

	

=======
>>>>>>> parent of 668f43b... Use opengl context rather than SDL window
	return true;
}

void Engine::run() {
	running = true;
	while (running) {
		flush_events();
		screen.update();
	}
}

void Engine::flush_events() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) running = false;
	}
}

void Engine::clean() {
	//
	// @Todo cleanup operations
	//
}