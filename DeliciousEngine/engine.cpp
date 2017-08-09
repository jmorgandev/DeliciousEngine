#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>

bool Engine::init(char** argv, int argc) {
	if (!console.init(this)) {
		return false;
	}
	if (!screen.init(this)) {
		return false;
	}
	if (!resources.init(this)) {
		return false;
	}
	
	//resources.load_shader("shader/font");
	//resources.load_texture("texture/con_font.png");

	resources.load_shader("res/default.glsl");
	return true;
}

void Engine::run() {
	running = true;

	Mesh* test = resources.fetch_mesh("quad");
	Shader* shad = resources.fetch_shader("res/default.glsl");
	//Shader* shad = resources.fetch_shader("res/default.glsl");
	if (shad == nullptr) return;
	if (test == nullptr) return;

	GLfloat bg_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	while (running) {
		glClearBufferfv(GL_COLOR, 0, bg_color);

		glUseProgram(shad->gpu_id);
		glBindVertexArray(test->vao);
		glDrawArrays(GL_TRIANGLES, 0, test->vertex_count);

		flush_events();
		screen.update();
	}

	glDeleteVertexArrays(1, &test->vao);
	glDeleteProgram(shad->gpu_id);
}

void Engine::flush_events() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) running = false;
	}
}

void Engine::clean() {
	//
	// @Todo cleanup operations
	//
}