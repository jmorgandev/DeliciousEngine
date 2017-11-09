#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include "console.h"
#include "screen.h"
#include "resources.h"
#include "input.h"
#include "world.h"

class Engine {
public:
	Engine();
	bool init(char** argv, int argc);
	void run();
	void shutdown();
private:
	system_var running;
	system_var strict_mode;

	Console	  console;
	Resources resources;
	Screen	  screen;
	Input	  input;
	World	  world;
};

#endif