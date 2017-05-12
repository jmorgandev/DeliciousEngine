#ifndef DELICIOUS_SCREEN_H
#define DELICIOUS_SCREEN_H

#include <SDL/SDL_video.h>

class Engine;
class Screen {
public:
	bool init(Engine* engine_in);

private:
	SDL_Window* window;
};

#endif