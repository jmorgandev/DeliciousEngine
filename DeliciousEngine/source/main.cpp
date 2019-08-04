#include <SDL_timer.h>

#include "console.h"
#include "screen.h"
#include "resources.h"
#include "input.h"
#include "world.h"
#include "engine.h"

static DeliciousEngine engine;
static Console   console(engine);
static Screen    screen(engine);
static Resources resources(engine);
static Input     input(engine);
static World     world(engine);

SystemVar eng_running = false;
SystemVar eng_strict = false;

#include <fmod.hpp>

static FMOD::System* fmod_system = nullptr;
static FMOD::Sound* sound = nullptr;
static FMOD::Channel* chan = nullptr;

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
	if (!console.load()) return false;
	if (!screen.load()) return false;
	if (!resources.load()) return false;
	if (!input.load()) return false;
	if (!world.load()) return false;
	return true;
}
static bool startup() {
	if (!screen.start()) return false;
	if (!resources.start()) return false;
	if (!world.start()) return false;

	console.register_variable("eng_running", &eng_running, CVAR_BOOL, CVAR_SYSTEM);
	console.register_variable("eng_strict",  &eng_strict,  CVAR_BOOL, CVAR_USER);

	return true;
}
static void shutdown() {
	world.free();
	input.free();
	resources.free();
	screen.free();
	console.free();
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