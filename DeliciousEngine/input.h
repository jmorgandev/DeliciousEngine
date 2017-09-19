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

	void bind(SDL_Keycode keycode, cstring command);
	void unbind(SDL_Keycode keycode);
private:
	System_Ref system;

	std::vector<key_bind> key_binds;
	std::vector<key_record> key_records;

	key_bind* find_bind(SDL_Keycode key);
	key_record* find_record(SDL_Keycode key);

	void update_records();
};

#endif