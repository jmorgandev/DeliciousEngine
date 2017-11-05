#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>
#include "font_renderer.h"
#include "dmath.h"
#include "system_ref.h"

bool Engine::init(char** argv, int argc) {
	System_Ref systems;
	systems.console	  = &console;
	systems.screen    = &screen;
	systems.resources = &resources;

	if (console.init(systems) == false) return false;
	if (screen.init(systems) == false) return false;
	if (resources.init(systems) == false) return false;
	if (input.init(systems) == false) return false;

	//console load config
	if (screen.create_window() == false) return false;
	if (resources.load_default_resources() == false) return false;

	console.register_variable("eng_running", &eng_running, CVAR_BOOL, CVAR_SYSTEM);
	console.register_variable("eng_strict",  &eng_strict,  CVAR_BOOL, CVAR_USER);

	return true;
}

void Engine::run() {
	eng_running = true;
	
	while (eng_running.as_bool == true) {
		input.process_events();
		console.render();
		screen.update();
	}
}

void Engine::clean() {
	//
	// @Todo cleanup operations
	//
}