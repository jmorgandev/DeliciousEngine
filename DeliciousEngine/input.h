#ifndef DELICIOUS_INPUT_H
#define DELICIOUS_INPUT_H

#include "system_ref.h"
#include "input_types.h"
#include "dtypes.h"

class Input {
public:
	bool init(System_Ref sys);

	void process_events();

	void bind(SDL_Keycode keycode, cstring command);
private:
	System_Ref system;
};

#endif