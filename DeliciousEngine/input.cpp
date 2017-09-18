#include "input.h"

#include "console.h"
#include "dcf.h"
#include <SDL/SDL_events.h>
#include <algorithm>

bool Input::init(System_Ref sys) {
	system = sys;

	bind(SDLK_BACKQUOTE, "con_toggle");
	bind(SDLK_ESCAPE, "quit");

	return true;
}

void Input::bind(SDL_Keycode keycode, cstring command) {
	for (auto keybind : key_bindings) {
		if (keybind.keycode == keycode) {
			dcf::str_cpy(command, keybind.command);
			return;
		}
	}
	key_bind new_binding = { keycode, "" };
	dcf::str_cpy(command, new_binding.command);
	key_bindings.push_back(new_binding);
}

void Input::unbind(SDL_Keycode keycode) {
	for (auto itr = key_bindings.begin(); itr != key_bindings.end(); itr++) {
		if (itr->keycode == keycode) {
			key_bindings.erase(itr);
			break;
		}
	}
}

void Input::process_events() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			system.console->write_variable("eng_running", false);
		}
		else system.console->input_event(e);
	}
}