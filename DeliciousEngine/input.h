#ifndef DELICIOUS_INPUT_H
#define DELICIOUS_INPUT_H

#include "system_ref.h"
#include "input_types.h"

#include <vector>
#include "dtypes.h"

class Input {
public:
	bool init(System_Ref sys);

	void process_events();

	bool get_keydown(SDL_Keycode keycode);
	bool get_keyup(SDL_Keycode keycode);
	bool get_keyhold(SDL_Keycode keycode);

	void bind(SDL_Keycode keycode, cstring command);
	void unbind(SDL_Keycode keycode);
private:
	System_Ref system;

	std::vector<key_bind> key_bindings;
	std::vector<key_record> key_stack;
};

#endif