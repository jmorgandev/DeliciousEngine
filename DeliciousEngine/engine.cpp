#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>
#include "font_renderer.h"
#include "dmath.h"

FontRenderer testRender;

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

	//resources.load_shader("res/default.glsl");
	//resources.load_texture("res/tiles.bmp");

	Shader* shad = resources.load_shader("res/bmp_font.glsl");
	Shader* shad2 = resources.load_shader("res/default.glsl");
	Texture* tex = resources.load_texture("res/consolas_32.tga");

	if (!shad || !shad2 || !tex) return false;

	resources.make_font("ubuntu", tex, shad, 32);
	return true;
}

void Engine::run() {
	running = true;

	testRender.set_font(resources.fetch_font("ubuntu"));
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Font* font = resources.fetch_font("ubuntu");
	Mesh* mesh = resources.fetch_mesh("quad");

	if (!font) return;

	GLfloat bg_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	while (running) {
		glClearBufferfv(GL_COLOR, 0, bg_color);

		static float t = 0;
		t += 0.01;

		testRender.begin();
		//testRender.draw_char('G', -0.5f, 0.0f);
		//testRender.draw_char('!', 0.5f, 0.0f);
		//testRender.draw_char(((int)t % 95) + ' ', 0.0f, 0.0f);
		testRender.draw_string("Hello!", 0.0f, 0.0f);
		//testRender.draw_string("Hello!", math::wrapf(7.0f - t, -1.0f, 1.0f), 0.0f);
		testRender.end();

		//glUseProgram(font->shader->id);
		//glBindTexture(GL_TEXTURE_2D, font->texture->id);
		//glBindVertexArray(font->gui_vao);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		flush_events();
		screen.update();
	}

	
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