#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>
#include "font_renderer.h"
#include "dmath.h"
#include "system_ref.h"

bool Engine::init(char** argv, int argc) {
	System_Ref sys;
	sys.console		= &console;
	sys.screen		= &screen;
	sys.resources	= &resources;

	if (!console.init(sys)) {
		return false;
	}
	if (!screen.init(sys)) {
		return false;
	}
	if (!resources.init(sys)) {
		return false;
	}
	return true;
}

void Engine::run() {
	running = true;

	GLfloat bg_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	SDL_StartTextInput();
	while (running) {
		glClearBufferfv(GL_COLOR, 0, bg_color);

		flush_events();
		console.render();
		screen.update();
	}
}

void Engine::flush_events() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) running = false;
		console.send_event(e);
	}
}

void Engine::clean() {
	//
	// @Todo cleanup operations
	//
}