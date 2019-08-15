#include "engine.h"

#include "console.h"
#include "screen.h"
#include "resources.h"
#include "world.h"
#include "input.h"

#include <SDL_timer.h>

/*
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
*/

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

int DeliciousEngine::run(int argc, char** argv) {
	if (!load())
		return 1;

	running = true;

	constexpr uint max_timestep = 1000 / 60;
	uint acc = 0;
	uint last_time = SDL_GetTicks();

	//do_fmod();
	while (running) {
		const uint current_time = SDL_GetTicks();
		acc += (current_time - last_time);
		last_time = current_time;

		get<Input>().process_events();
		get<Screen>().begin_gui();
		while (acc >= max_timestep) {
			get<World>().update();
			acc -= max_timestep;
		}
		get<Screen>().render_frame();
	}
	//exit_fmod();

	return 0;
}

bool DeliciousEngine::load() {

	add_default_modules();

	for (auto id : module_load_order) {
		if (!modules[id]->load())
			return false;
	}

	for (auto id : module_load_order) {
		if (!modules[id]->start())
			return false;
	}

	return true;
}

void DeliciousEngine::add_default_modules() {
	add<Console>();
	add<Screen>();
	add<Resources>();
	add<Input>();
	add<World>();
}

DeliciousEngine::~DeliciousEngine() {
	for (auto itr = module_load_order.rbegin(); itr != module_load_order.rend(); itr++) {
		modules[*itr]->free();
		delete modules[*itr];
	}
	modules.clear();
}