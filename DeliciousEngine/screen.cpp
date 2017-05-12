#include "screen.h"

#include <SDL/SDL.h>
#include <iostream>
#include "engine.h"

bool Screen::init(Engine* engine_in) {
	window = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL could not be initialised: " << SDL_GetError() << "\n";
		return false;
	}

	window = SDL_CreateWindow(
		"Window Test!",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280, 720, SDL_WINDOW_SHOWN
	);
	if (!window) {
		std::cout << "SDL window could not be created: " << SDL_GetError() << "\n";
		return false;
	}
	SDL_Delay(2000);
	return true;
}