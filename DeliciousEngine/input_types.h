#ifndef DELICIOUS_INPUT_TYPES_H
#define DELICIOUS_INPUT_TYPES_H

#include <SDL/SDL_keycode.h>
#include "dtypes.h"

#define INPUT_MAX_NAME 32

enum key_state {
	KEY_PRESSED,
	KEY_RELEASED,
	KEY_HELD
};
struct key_record {
	SDL_Keycode keycode;
	key_state state;
};
struct key_bind {
	SDL_Keycode keycode;
	cstring command;
};

#endif