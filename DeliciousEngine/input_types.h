#ifndef DELICIOUS_INPUT_TYPES_H
#define DELICIOUS_INPUT_TYPES_H

#include <SDL/SDL_keycode.h>
#include "console_types.h"
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
	char command[CON_MAX_NAME];
};

#endif