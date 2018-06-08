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

#include <fmod.hpp>

FMOD::System* fmod_system = nullptr;
FMOD::Sound* sound = nullptr;
FMOD::Channel* chan = nullptr;

static void do_fmod() {
	FMOD_RESULT result;
	
	result = FMOD::System_Create(&fmod_system);
	result = fmod_system->init(8, FMOD_INIT_NORMAL, nullptr);

	fmod_system->createSound("res/swish.wav", FMOD_DEFAULT, nullptr, &sound);
	sound->setMode(FMOD_LOOP_BIDI);

	fmod_system->playSound(sound, nullptr, false, &chan);
}
static void exit_fmod() {
	chan->stop();
	sound->release();
	fmod_system->release();
}

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
	if (!world.load_test()) return false;
	scripting.load_start_script();

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
	do_fmod();
	if (!init_systems()) return 1;
	if (startup()) {
		eng_running = true;
		const uint max_timestep = 1000 / 60;
		uint acc = 0;
		uint last_time = SDL_GetTicks();
		fmod_system->update();
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
	exit_fmod();
	shutdown();

	return 0;
}