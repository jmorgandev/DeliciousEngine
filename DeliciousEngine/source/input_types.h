#ifndef DELICIOUS_INPUT_TYPES_H
#define DELICIOUS_INPUT_TYPES_H

#include <functional>

#include <SDL_keycode.h>
#include "dtypes.h"

enum key_state {
	KEY_PRESSED,
	KEY_RELEASED,
	KEY_HOLD
};
struct key_record {
	SDL_Keycode keycode;
	key_state state;
};

struct key_bind {
	SDL_Keycode keycode;
	std::function<void(void)> lambda;
};

//@Todo: User defined buttons

#endif