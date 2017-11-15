#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include "build_info.h"

#include "console.h"
#include "screen.h"
#include "resources.h"
#include "input.h"
#include "world.h"

#if EXPOSE_GLOBAL_SYSTEM
extern System_Ref global_system;
#endif

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