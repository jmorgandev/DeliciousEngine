#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>
#include "font_renderer.h"
#include "dmath.h"

bool Engine::init(char** argv, int argc) {
	if (!console.init(this)) {
		return false;
	}
	if (!screen.init(this)) {
		return false;
	}
	if (!resources.init(this)) {
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
		if (e.type == SDL_KEYDOWN) console.key_event(e.key);
		if (e.type == SDL_TEXTINPUT) console.text_event(e.text);
	}
}

void Engine::clean() {
	//
	// @Todo cleanup operations
	//
}