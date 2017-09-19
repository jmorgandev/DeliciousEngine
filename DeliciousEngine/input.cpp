#include "input.h"

#include "console.h"
#include "dcf.h"
#include <SDL/SDL_events.h>
#include <algorithm>

bool Input::init(System_Ref sys) {
	system = sys;

	key_records.reserve(10);

	bind(SDLK_BACKQUOTE, "toggleconsole");
	bind(SDLK_ESCAPE, "quit");

	return true;
}

void Input::bind(SDL_Keycode keycode, cstring command) {
	for (auto bind : key_binds) {
		if (bind.keycode == keycode) {
			dcf::str_cpy(command, bind.command);
			return;
		}
	}
	key_bind new_binding = { keycode, "" };
	dcf::str_cpy(command, new_binding.command);
	key_binds.push_back(new_binding);
}

void Input::unbind(SDL_Keycode keycode) {
	for (auto itr = key_binds.begin(); itr != key_binds.end(); itr++) {
		if (itr->keycode == keycode) {
			key_binds.erase(itr);
			break;
		}
	}
}

void Input::process_events() {
	update_records();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			system.console->write_variable("eng_running", false);
			break;
		case SDL_KEYDOWN:
			if (key_record* record = find_record(event.key.keysym.sym)) {
				record->state = KEY_HOLD;
			}
			else {
				if (key_bind* bind = find_bind(event.key.keysym.sym)) {
					//call the keybind console command
				}
				else if (system.console->is_open()) {
					system.console->input_event(event);
				}
				else {
					key_record new_record = { event.key.keysym.sym, KEY_PRESSED };
					key_records.push_back(new_record);
				}
			}
			break;
		case SDL_KEYUP:
			if (key_record* record = find_record(event.key.keysym.sym)) {
				record->state = KEY_RELEASED;
			}
			break;
		}
	}
}

key_bind* Input::find_bind(SDL_Keycode key) {
	for (auto itr = key_binds.begin(); itr != key_binds.end(); itr++) {
		if (itr->keycode == key) {
			return &(*itr);
		}
	}
	return nullptr;
}

key_record* Input::find_record(SDL_Keycode key) {
	for (auto itr = key_records.begin(); itr != key_records.end(); itr++) {
		if (itr->keycode == key) {
			return &(*itr);
		}
	}
	return nullptr;
}

void Input::update_records() {
	for (auto itr = key_records.begin(); itr != key_records.end();) {
		if (itr->state == KEY_RELEASED) {
			itr = key_records.erase(itr);
		}
		else {
			itr++;
		}
	}
}