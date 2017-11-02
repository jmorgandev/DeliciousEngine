#ifndef DELICIOUS_SCREEN_H
#define DELICIOUS_SCREEN_H

#include <SDL/SDL_video.h>
#include <GL/glew.h>

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

	void resize(int width, int height);
private:
	System_Ref system;

	SDL_Window* window;
	SDL_GLContext gl_context;

	GLuint gui_vertex_array;
	GLuint gui_vertex_buffers[2];

	system_var vid_init = false;
	system_var vid_width = 800;
	system_var vid_height = 600;
	system_var vid_fullscreen = false;
	system_var vid_borderless = false;
};

#endif