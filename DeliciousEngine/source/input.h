#ifndef DELICIOUS_INPUT_H
#define DELICIOUS_INPUT_H

#include <functional>
#include <vector>

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

	void bind(SDL_Keycode keycode, std::function<void(void)> lambda);
	void unbind(SDL_Keycode keycode);
private:
	std::vector<key_bind> key_binds;
	std::vector<key_record> key_records;

	bool mouse_buttons[5];
	glm::vec2 mouse_motion;

	key_bind* find_bind(SDL_Keycode key);
	key_record* find_record(SDL_Keycode key);

	void update_records();
	void setup_gui_bindings();
};

#endif