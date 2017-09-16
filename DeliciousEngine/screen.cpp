#include "screen.h"

#include <SDL/SDL.h>
#include <iostream>
#include "console.h"
#include "build_info.h"

bool Screen::init(System_Ref sys) {
	system = sys;
	window = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL could not be initialised: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ENGINE_GL_MAJOR);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ENGINE_GL_MINOR);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glEnable(GL_DEPTH_TEST);

	int video_width = sys.console->find_variable("vid_width")->value.as_int;
	int video_height = sys.console->find_variable("vid_height")->value.as_int;

	window = SDL_CreateWindow(
		"Window Test!",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		video_width, video_height,
		SDL_WINDOW_OPENGL
	);
	screen_width = video_width;
	screen_height = video_height;
	if (!window) {
		std::cout << "SDL window could not be created: " << SDL_GetError() << "\n";
		return false;
	}

	gl_context = SDL_GL_CreateContext(window);
	if (!gl_context) {
		std::cout << "OpenGL context could not be created: " << SDL_GetError() << "\n";
		return false;
	}
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cout << "GLEW failed to initialise: " << glewGetErrorString(status) << "\n";
		return false;
	}
	glewExperimental = true;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}

const GLfloat bg_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

void Screen::update() {

	

	SDL_GL_SwapWindow(window);
}

void Screen::refresh() {
	if (window != NULL) {
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		window = NULL;
	}

	int video_width = system.console->find_variable("vid_width")->value.as_int;
	int video_height = system.console->find_variable("vid_height")->value.as_int;

	window = SDL_CreateWindow(
		"Window Test!",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		video_width, video_height,
		SDL_WINDOW_OPENGL
	);
	screen_width = video_width;
	screen_height = video_height;

	if (!window) {
		std::cout << "Window could not be refreshed: " << SDL_GetError() << '\n';
		while (true);
	}
	gl_context = SDL_GL_CreateContext(window);
	if (!gl_context) {
		std::cout << "GL context could not be refreshed: " << SDL_GetError() << '\n';
		while (true);
	}

	glewInit();
}

bool Screen::create_window() {
	if (window != nullptr) {
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		gl_context = nullptr;
		window = nullptr;
	}


}

int Screen::get_width() {
	return screen_width;
}
int Screen::get_height() {
	return screen_height;
}