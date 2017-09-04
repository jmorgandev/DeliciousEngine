#ifndef DELICIOUS_SCREEN_H
#define DELICIOUS_SCREEN_H

#include <SDL/SDL_video.h>

#include "system_ref.h"

class Screen {
public:
	bool init(System_Ref sys);

	void update();

	int get_width();
	int get_height();

private:
	SDL_Window* window;
	SDL_GLContext gl_context;
	int screen_width;
	int screen_height;
};

#endif