#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>
#include "mesh_renderer.h"
#include "dmath.h"
#include "system_ref.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//@TEMP Move time logic into time subsystem
#include <chrono>
#include <ratio>
using namespace std::chrono;

Engine::Engine() {
	running = false;
	strict_mode = false;
}

bool Engine::init(char** argv, int argc) {
	//@TODO: Add physics?
	System_Ref systems;
	systems.console	  = &console;
	systems.screen    = &screen;
	systems.resources = &resources;
	systems.input     = &input;
	systems.world     = &world;
	systems.time	  = &time;

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
	if (time.init(systems) == false) return false;
	if (world.init(systems) == false) return false;

	//@TODO: Load config file with console
	if (screen.create_window() == false) return false;
	if (resources.load_default_resources() == false) return false;
	
	if (world.load_test() == false) return false;

	console.register_variable("eng_running",    &running,     CVAR_BOOL, CVAR_SYSTEM);
	console.register_variable("eng_strict",     &strict_mode, CVAR_BOOL, CVAR_USER);

#if EXPOSE_GLOBAL_SYSTEM
	global_system = systems;
#endif

	return true;
}

//@TEMP Move time logic into time subsystem
using hrc = high_resolution_clock;

void Engine::run() {
	running = true;

	time.start();

	//@TODO decouple world simulation and rendering, current this is just a framerate limiter
	//      that locks simulation AND rendering at a maximum of 60hz.
	const float max_timestep = 1.0f / 60.0f;
	float acc = 0.0f;
	auto last_time = hrc::now();

	while (running.as_bool == true) {
		const auto current_time = hrc::now();
		const duration<float> frame_time = current_time - last_time;
		last_time = current_time;
		acc += frame_time.count();

		input.process_events();
		while (acc >= max_timestep) {
			world.update();
			screen.render_frame();
			acc -= max_timestep;
		}
	}
}

void Engine::shutdown() {
	world.clean_exit();
	time.clean_exit();
	input.clean_exit();
	resources.clean_exit();
	screen.clean_exit();
	console.clean_exit();
}

#if EXPOSE_GLOBAL_SYSTEM
System_Ref global_system;
#endif