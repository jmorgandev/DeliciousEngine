#ifndef DELICIOUS_INPUT_H
#define DELICIOUS_INPUT_H

#include "input_types.h"

#include <vector>
#include "dtypes.h"

class Input {
public:
	bool init();
	void clean_exit();

	void process_events();

	bool get_key(SDL_Keycode keycode);

	void bind(SDL_Keycode keycode, cstring command);
	void unbind(SDL_Keycode keycode);
private:

	std::vector<key_bind> key_binds;
	std::vector<key_record> key_records;

	key_bind* find_bind(SDL_Keycode key);
	key_record* find_record(SDL_Keycode key);

	void update_records();
};
extern Input* input;

#endif