#include "input.h"

#include "console.h"
#include "dcf.h"
#include <SDL/SDL_events.h>
#include <algorithm>

bool Input::init(System_Ref sys) {
	system = sys;

	key_records.reserve(10);

	//@TEMP
	bind(SDLK_BACKQUOTE, "toggleconsole");

	return true;
}

void Input::clean_exit() {
	//@TODO: Dump keybinds to config files.
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
					system.console->execute_keybind(bind);
				}
				else {
					key_record new_record = { event.key.keysym.sym, KEY_PRESSED };
					key_records.push_back(new_record);
				}
			}
			if (system.console->is_open()) {
				system.console->key_input(event.key);
			}
			break;
		case SDL_KEYUP:
			if (key_record* record = find_record(event.key.keysym.sym)) {
				record->state = KEY_RELEASED;
			}
			break;
		case SDL_TEXTINPUT:
			if (system.console->is_open()) {
				system.console->text_input(event.text);
			}
			break;
		}
	}
}

key_bind* Input::find_bind(SDL_Keycode key) {
	for (auto it = key_binds.begin(); it != key_binds.end(); it++) {
		if (it->keycode == key) {
			return &(*it);
		}
	}
	return nullptr;
}

key_record* Input::find_record(SDL_Keycode key) {
	for (auto it = key_records.begin(); it != key_records.end(); it++) {
		if (it->keycode == key) {
			return &(*it);
		}
	}
	return nullptr;
}

void Input::update_records() {
	for (auto it = key_records.begin(); it != key_records.end();) {
		if (it->state == KEY_RELEASED) {
			it = key_records.erase(it);
		}
		else {
			it++;
		}
	}
}