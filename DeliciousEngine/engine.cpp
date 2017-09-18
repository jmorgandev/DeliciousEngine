#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>
#include "font_renderer.h"
#include "dmath.h"
#include "system_ref.h"

bool Engine::init(char** argv, int argc) {
	System_Ref systems;
	systems.console	= &console;
	systems.screen = &screen;
	systems.resources = &resources;

	if (console.init(systems) == false) return false;
	if (screen.init(systems) == false) return false;
	if (resources.init(systems) == false) return false;
	if (input.init(systems) == false) return false;

	//console load config
	if (screen.create_window() == false) return false;
	if (resources.load_default_resources() == false) return false;

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
		console.input_event(e);
	}
}

void Engine::clean() {
	//
	// @Todo cleanup operations
	//
}