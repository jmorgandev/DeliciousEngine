#include "screen.h"

#include <SDL/SDL.h>
#include <iostream>
#include "engine.h"

bool Screen::init(Engine* engine_in) {
	window = NULL;
	Console* console_ref = engine_in->get_console();

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL could not be initialised: " << SDL_GetError() << "\n";
		return false;
	}

	int video_width = static_cast<int>(console_ref->read_variable("vid_width"));
	int video_height = static_cast<int>(console_ref->read_variable("vid_height"));

	window = SDL_CreateWindow(
		"Window Test!",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		video_width, video_height,
		SDL_WINDOW_SHOWN
	);
	if (!window) {
		std::cout << "SDL window could not be created: " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}

void Screen::update() {
	//
	// @Todo OpenGL rendering stuff here
	//
}