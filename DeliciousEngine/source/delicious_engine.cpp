#include "delicious_engine.h"

#include "console.h"
#include "screen.h"
#include "resources.h"
#include "world.h"
#include "input.h"

bool DeliciousEngine::launch(const Game& game) {
	//Console   console;
	//Screen    screen;
	//Resources resources;
	//Input     input;
	//World     world;

	engine_systems.emplace_back(Console());
	engine_systems.emplace_back(Screen());
	engine_systems.emplace_back(Resources());
	engine_systems.emplace_back(Input());
	engine_systems.emplace_back(World());

	return true;
}