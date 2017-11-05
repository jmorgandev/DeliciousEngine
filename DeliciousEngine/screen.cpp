#include "screen.h"
#include "screen_types.h"

#include <SDL/SDL.h>
#include <iostream>
#include "console.h"
#include "build_info.h"

//@TEMP
GLfloat bg_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

Screen::Screen() {
	window = nullptr;
	gl_context = nullptr;
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

	glEnable(GL_DEPTH_TEST);
	glClearBufferfv(GL_COLOR, 0, bg_color);

	console.register_variable("vid_init",       &vid_init,       CVAR_BOOL, CVAR_SYSTEM);
	console.register_variable("vid_width",      &vid_width,      CVAR_INT,  CVAR_CONFIG);
	console.register_variable("vid_height",     &vid_height,     CVAR_INT,  CVAR_CONFIG);
	console.register_variable("vid_fullscreen", &vid_fullscreen, CVAR_BOOL, CVAR_CONFIG);
	console.register_variable("vid_borderless", &vid_borderless, CVAR_BOOL, CVAR_CONFIG);

	return true;
}

bool Screen::create_window() {
	Console& console = *system.console;

	uint32 sdl_flags = SDL_WINDOW_OPENGL;
	if (vid_fullscreen.as_bool == true && vid_borderless.as_bool == true) {
		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) == 0) {
			vid_width.as_int = dm.w;
			vid_height.as_int = dm.h;
			sdl_flags |= SDL_WINDOW_BORDERLESS;
		}
		else {
			console << "Cannot detect native resolution for borderless fullscreen, reverting to windowed mode.\n";
			vid_fullscreen = false;
			vid_borderless = false;
		}
	}
	else if (vid_fullscreen.as_bool == true) {
		sdl_flags |= SDL_WINDOW_FULLSCREEN;
	}
	else if (vid_borderless.as_bool == true) {
		sdl_flags |= SDL_WINDOW_BORDERLESS;
	}

	window = SDL_CreateWindow(
		"Delicious Engine " ENGINE_VERSION_STRING,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		vid_width.as_int,
		vid_height.as_int,
		sdl_flags
	);
	if (window == nullptr) {
		console << "SDL window could not be created: " << SDL_GetError() << "\n";
		return false;
	}

	if (gl_context != nullptr) {
		if (SDL_GL_MakeCurrent(window, gl_context) != 0) {
			console << "Could not attach existing GL context to SDL window: " << SDL_GetError() << "\n";
			return false;
		}
		else {
			glViewport(0, 0, vid_width.as_int, vid_height.as_int);
		}
	}
	else {
		gl_context = SDL_GL_CreateContext(window);
		if (gl_context == nullptr) {
			console << "SDL_GL context could not be created: " << SDL_GetError() << "\n";
			return false;
		}
	}

	GLenum status = glewInit();
	if (status == GLEW_OK) {
		glewExperimental = true;
	}
	else {
		console << "GLEW could not be initialised: " << glewGetErrorString(status) << "\n";
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	vid_init = true;

	return true;
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
	glClearBufferfv(GL_COLOR, 0, bg_color);
}

int Screen::get_width() {
	return vid_width.as_int;
}
int Screen::get_height() {
	return vid_height.as_int;
}

void Screen::resize(int width, int height) {
	vid_width = width;
	vid_height = height;
	reload_window();
	system.console->display_reformat();
}