#include "input.h"

bool Input::init(System_Ref sys) {
	system = sys;

	return true;
}

void Input::bind(SDL_Keycode keycode, cstring command) {
	
}