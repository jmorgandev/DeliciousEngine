#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include "system.h"
#include <vector>

class Game;

class DeliciousEngine {
public:
	bool launch(const Game& game);

private:
	std::vector<System> engine_systems;
};

#endif