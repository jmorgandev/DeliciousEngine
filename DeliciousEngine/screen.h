#ifndef DELICIOUS_SCREEN_H
#define DELICIOUS_SCREEN_H

#include <SDL/SDL_video.h>

#include "system_ref.h"
#include "system_var.h"

class Screen {
public:
	Screen();
	bool init(System_Ref sys);

	void update();
	bool create_window();
	bool reload_window();

	int get_width();
	int get_height();
private:
	System_Ref system;

	SDL_Window* window;
	SDL_GLContext gl_context;

	system_var vid_init = false;
	system_var vid_width = 800;
	system_var vid_height = 600;
	system_var vid_fullscreen = false;
	system_var vid_borderless = false;
};

#endif