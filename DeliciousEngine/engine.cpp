#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>
#include "mesh_renderer.h"
#include "dmath.h"
#include "system_ref.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Engine::Engine() {
	running = false;
	strict_mode  = false;
}

bool Engine::init(char** argv, int argc) {
	System_Ref systems;
	systems.console	  = &console;
	systems.screen    = &screen;
	systems.resources = &resources;
	systems.input     = &input;
	systems.world     = &world;

	//@TODO: Parse command line arguments.
	//	--strict   = Start the engine in strict mode
	//  --gl <num> = Override the OpenGL context version
	//  --fast     = Forces all graphics settings to their lowest/fastest
	//  --pretty   = Forces all graphics settings to their highest
	//  --dev	   = Load a blank world and open the console on startup

	if (console.init(systems) == false) return false;
	if (screen.init(systems) == false) return false;
	if (resources.init(systems) == false) return false;
	if (input.init(systems) == false) return false;
	if (world.init(systems) == false) return false;

	//@TODO: Load config file with console
	if (screen.create_window() == false) return false;
	if (resources.load_default_resources() == false) return false;
	
	if (world.load_test() == false) return false;

	console.register_variable("eng_running", &running,     CVAR_BOOL, CVAR_SYSTEM);
	console.register_variable("eng_strict",  &strict_mode, CVAR_BOOL, CVAR_USER);

#if EXPOSE_GLOBAL_SYSTEM
	global_system = systems;
#endif
	return true;
}

void Engine::run() {
	running = true;

	while (running.as_bool == true) {
		input.process_events();

		world.update();

		screen.render_frame();
	}
}

void Engine::shutdown() {
	world.clean_exit();
	input.clean_exit();
	resources.clean_exit();
	screen.clean_exit();
	console.clean_exit();
}

#if EXPOSE_GLOBAL_SYSTEM
System_Ref global_system;
#endif