#include "engine.h"

#include <SDL/SDL_events.h>
#include <SDL/SDL_timer.h>
#include <fstream>
#include "mesh_renderer.h"
#include "dmath.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Engine::Engine() {
	running = false;
	strict_mode = false;
}

bool Engine::init(char** argv, int argc) {
	console	  = &eng_console;
	screen    = &eng_screen;
	resources = &eng_resources;
	input     = &eng_input;
	scripting = &eng_scripting;
	world     = &eng_world;
	physics   = &eng_physics;

	//@TODO: Parse command line arguments.
	//	--strict   = Start the engine in strict mode
	//  --gl <num> = Override the OpenGL context version
	//  --fast     = Forces all graphics settings to their lowest/fastest
	//  --pretty   = Forces all graphics settings to their highest
	//  --dev	   = Load a blank world and open the console on startup

	if (console->init() == false) return false;
	if (screen->init() == false) return false;
	if (resources->init() == false) return false;
	if (input->init() == false) return false;
	if (scripting->init() == false) return false;
	if (world->init() == false) return false;

	//@TODO: Load config file with console
	if (screen->create_window() == false) return false;
	if (resources->load_default_resources() == false) return false;
	
	if (world->load_test() == false) return false;

	console->register_variable("eng_running",    &running,     CVAR_BOOL, CVAR_SYSTEM);
	console->register_variable("eng_strict",     &strict_mode, CVAR_BOOL, CVAR_USER);

	console->display(false);

	return true;
}

void Engine::run() {
	running = true;

	//@TODO decouple world simulation and rendering, current this is just a framerate limiter
	//      that locks simulation AND rendering at a maximum of 60hz.
	const uint max_timestep = 1000 / 60;
	uint accumulator = 0;
	uint last_time = SDL_GetTicks();

	while (running.as_bool == true) {
		const uint current_time = SDL_GetTicks();
		accumulator += (current_time - last_time);
		last_time = current_time;

		while (accumulator >= max_timestep) {
			input->process_events();
			world->update();
			screen->render_frame();
			accumulator -= max_timestep;
		}
	}
}

void Engine::shutdown() {
	world->clean_exit();
	scripting->clean_exit();
	input->clean_exit();
	resources->clean_exit();
	screen->clean_exit();
	console->clean_exit();
}

Console*	console;
Screen*		screen;
Resources*	resources;
Input*		input;
Scripting*	scripting;
World*		world;
Physics*	physics;