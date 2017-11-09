#include "screen.h"
#include "screen_types.h"

#include <SDL/SDL.h>
#include <iostream>
#include "console.h"
#include "build_info.h"

//@TEMP
GLfloat bg_color[] = { 0.2f, 0.1f, 0.3f, 1.0f };

Screen::Screen() {
	window = nullptr;
	gl_context = nullptr;

	init_success  = false;
	width         = 800;
	height        = 600;
	fullscreen    = false;
	borderless    = false;
	field_of_view = 75.0f;
	aspect_ratio  = 4.0f / 3.0f;
}

bool Screen::init(System_Ref sys) {
	system = sys;
	Console& console = *system.console;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		console << "SDL could not be initialised: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ENGINE_GL_MAJOR);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ENGINE_GL_MINOR);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,  32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	console.register_variable("vid_init",       &init_success,  CVAR_BOOL,  CVAR_SYSTEM);
	console.register_variable("vid_width",      &width,         CVAR_INT,   CVAR_CONFIG);
	console.register_variable("vid_height",     &height,        CVAR_INT,   CVAR_CONFIG);
	console.register_variable("vid_fullscreen", &fullscreen,    CVAR_BOOL,  CVAR_CONFIG);
	console.register_variable("vid_borderless", &borderless,    CVAR_BOOL,  CVAR_CONFIG);
	console.register_variable("vid_fov",		&field_of_view, CVAR_FLOAT, CVAR_USER  );
	console.register_variable("vid_aspect",     &aspect_ratio,  CVAR_FLOAT, CVAR_SYSTEM);

	return true;
}

void Screen::destroy() {
	if (gl_context != nullptr) {
		SDL_GL_DeleteContext(gl_context);
	}
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}

bool Screen::create_window() {
	init_success = false;
	Console& console = *system.console;

	uint32 sdl_flags = SDL_WINDOW_OPENGL;
	if (fullscreen.as_bool == true && borderless.as_bool == true) {
		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) == 0) {
			width.as_int  = dm.w;
			height.as_int = dm.h;
			sdl_flags |= SDL_WINDOW_BORDERLESS;
		}
		else {
			console << "Cannot detect native resolution for borderless fullscreen, reverting to windowed mode.\n";
			fullscreen = false;
			borderless = false;
		}
	}
	else if (fullscreen.as_bool == true) {
		sdl_flags |= SDL_WINDOW_FULLSCREEN;
	}
	else if (borderless.as_bool == true) {
		sdl_flags |= SDL_WINDOW_BORDERLESS;
	}

	window = SDL_CreateWindow(
		"Delicious Engine " ENGINE_VERSION_STRING,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width.as_int,
		height.as_int,
		sdl_flags
	);
	if (window == nullptr) {
		console << "SDL window could not be created: " << SDL_GetError() << "\n";
		return init_success.as_bool;
	}

	if (gl_context != nullptr) {
		if (SDL_GL_MakeCurrent(window, gl_context) != 0) {
			console << "Could not attach existing GL context to SDL window: " << SDL_GetError() << "\n";
			return init_success.as_bool;
		}
		else {
			glViewport(0, 0, width.as_int, height.as_int);
		}
	}
	else {
		gl_context = SDL_GL_CreateContext(window);
		if (gl_context == nullptr) {
			console << "SDL_GL context could not be created: " << SDL_GetError() << "\n";
			return init_success.as_bool;
		}
	}

	GLenum status = glewInit();
	if (status == GLEW_OK) {
		glewExperimental = true;
	}
	else {
		console << "GLEW could not be initialised: " << glewGetErrorString(status) << "\n";
		return init_success.as_bool;
	}
	init_success = true;

	aspect_ratio = (float)width.as_int / (float)height.as_int;
	glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	//current_camera.calculate_projection(vid_fov.as_float, get_aspect_ratio());

	return init_success.as_bool;
}

bool Screen::reload_window() {
	if (window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	return create_window();
}

void Screen::update() {
	SDL_GL_SwapWindow(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//current_camera.calculate_projection(vid_fov.as_float, get_aspect_ratio());
}

int Screen::get_width() {
	return width.as_int;
}
int Screen::get_height() {
	return height.as_int;
}

void Screen::resize(int new_width, int new_height) {
	width = new_width;
	height = new_height;
	reload_window();
	system.console->display_reformat();
}