#include <SDL_timer.h>

#include "console.h"
#include "screen.h"
#include "resources.h"
#include "input.h"
#include "scripting.h"
#include "world.h"

Console   console;
Screen    screen;
Resources resources;
Input     input;
Scripting scripting;
World     world;

SystemVar eng_running = false;
SystemVar eng_strict = false;

static bool init_systems() {
	if (!console.init()) return false;
	if (!screen.init()) return false;
	if (!resources.init()) return false;
	if (!input.init()) return false;
	if (!scripting.init()) return false;
	if (!world.init()) return false;
	return true;
}
static bool startup() {
	if (!screen.create_window()) return false;
	if (!resources.load_default_resources()) return false;
	scripting.load_start_script();
	if (!world.load_test()) return false;

	console.register_variable("eng_running", &eng_running, CVAR_BOOL, CVAR_SYSTEM);
	console.register_variable("eng_strict",  &eng_strict,  CVAR_BOOL, CVAR_USER);

	return true;
}
static void shutdown() {
	world.clean_exit();
	scripting.clean_exit();
	input.clean_exit();
	resources.clean_exit();
	screen.clean_exit();
	console.clean_exit();
}

int main(char** argv, int argc) {
	
	if (!init_systems()) return 1;
	if (startup()) {
		eng_running = true;
		const uint max_timestep = 1000 / 60;
		uint acc = 0;
		uint last_time = SDL_GetTicks();

		while (eng_running.as_bool) {
			const uint current_time = SDL_GetTicks();
			acc += (current_time - last_time);
			last_time = current_time;

			input.process_events();
			screen.begin_gui();
			while (acc >= max_timestep) {
				world.update();
				acc -= max_timestep;
			}
			screen.render_frame();
		}

	}
	shutdown();

	return 0;
}