#ifndef DELICIOUS_SCREEN_H
#define DELICIOUS_SCREEN_H

#include <SDL/SDL_video.h>

class Engine;
class Screen {
public:
	bool init(Engine* engine_in);

	void update();

private:
	SDL_Window* window;
	SDL_GLContext gl_context;
};

#endif