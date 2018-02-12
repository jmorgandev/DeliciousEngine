#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include "build_info.h"

#include "console.h"
#include "screen.h"
#include "resources.h"
#include "input.h"
#include "world.h"
#include "physics.h"
#include "scripting.h"

class Engine {
public:
	Engine();
	bool init(char** argv, int argc);
	void run();
	void shutdown();
private:
	system_var running;
	system_var strict_mode;

	Console	  eng_console;
	Resources eng_resources;
	Screen	  eng_screen;
	Input	  eng_input;
	Scripting eng_scripting;
	World	  eng_world;
	Physics   eng_physics;
};

#endif