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
	void clean_exit();

	void clear_frame();
	void render_frame();
	
	bool create_window();
	bool reload_window();

	int get_width();
	int get_height();

	void resize(int width, int height);
	float get_aspect_ratio() { return (float)width.as_int / (float)height.as_int; }

	Camera* get_camera();
private:
	System_Ref system;

	SDL_Window*   window;
	SDL_GLContext gl_context;

	GLuint gui_vertex_array;
	GLuint gui_vertex_buffers[2];

	system_var init_success;
	system_var width;
	system_var height;
	system_var fullscreen;
	system_var borderless;
	system_var field_of_view;
	system_var aspect_ratio;
	
	float near_plane;
	float far_plane;

	Camera camera;
};

#endif