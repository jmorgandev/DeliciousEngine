#ifndef DELICIOUS_SCREEN_H
#define DELICIOUS_SCREEN_H

#include <SDL/SDL_video.h>
#include <GL/glew.h>

#include "system_ref.h"
#include "system_var.h"
#include "camera.h"

//@TEMP
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

class Screen {
public:
	Screen();
	bool init(System_Ref sys);
	void stop();

	void update();
	bool create_window();
	bool reload_window();

	int get_width();
	int get_height();

	void resize(int width, int height);
	float get_aspect_ratio() { return (float)vid_width.as_int / (float)vid_height.as_int; }
private:
	System_Ref system;

	SDL_Window* window;
	SDL_GLContext gl_context;

	GLuint gui_vertex_array;
	GLuint gui_vertex_buffers[2];

	system_var vid_init;
	system_var vid_width;
	system_var vid_height;
	system_var vid_fullscreen;
	system_var vid_borderless;
	system_var vid_fov;
	
	float near_plane;
	float far_plane;
	float aspect_ratio;

	Camera current_camera;
};

#endif