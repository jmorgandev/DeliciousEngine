#ifndef DELICIOUS_INPUT_H
#define DELICIOUS_INPUT_H

#include <functional>
#include <vector>
#include <string>
#include <unordered_map>

#include <SDL_keycode.h>
#include <vec2.hpp>

#include "input_types.h"
#include "module.h"


class Input : public Module {
public:
	using Module::Module;

	bool load() override;
	void update() override { process_events(); }
	bool free() override;

	void process_events();

	bool get_key(SDL_Keycode keycode);
	bool key_pressed(SDL_Keycode keycode);
	bool key_held(SDL_Keycode keycode);
	bool key_released(SDL_Keycode keycode);
private:
	std::vector<KeyRecord> records;
	std::unordered_map<std::string, SDL_Keycode> buttons;

	bool mouse_buttons[5];
	glm::vec2 mouse_motion;

	KeyRecord* find_record(SDL_Keycode key);
	bool check_key_state(SDL_Keycode key, KeyState state);

	void flush();
	void setup_gui_bindings();
};

#endif