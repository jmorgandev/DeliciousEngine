#ifndef DELICIOUS_INPUT_TYPES_H
#define DELICIOUS_INPUT_TYPES_H

#include <functional>

#include <SDL_keycode.h>
#include "dtypes.h"

enum KeyState {
	KEY_PRESSED,
	KEY_RELEASED,
	KEY_HELD
};

struct KeyRecord {
	SDL_Keycode keycode;
	KeyState state;
};

struct key_bind {
	SDL_Keycode keycode;
	std::function<void(void)> callback;
};

//@Todo: User defined buttons

#endif