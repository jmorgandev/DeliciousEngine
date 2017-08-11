#include "screen.h"

#include <SDL/SDL.h>
#include <iostream>
#include "engine.h"

bool Screen::init(Engine* engine_in) {
	window = NULL;
	Console* console_ref = engine_in->get_console();

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL could not be initialised: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glEnable(GL_DEPTH_TEST);

	int video_width = static_cast<int>(console_ref->read_variable("vid_width"));
	int video_height = static_cast<int>(console_ref->read_variable("vid_height"));

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

int Screen::get_width() {
	return screen_width;
}
int Screen::get_height() {
	return screen_height;
}