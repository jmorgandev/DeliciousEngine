#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include "console.h"
#include "screen.h"
#include "resources.h"
#include "input.h"

class Engine {
public:
	Engine();
	bool init(char** argv, int argc);
	void run();
	void shutdown();
private:
	system_var eng_running;
	system_var eng_strict;

	Console		console;
	Resources	resources;
	Screen		screen;
	Input		input;
};

#endif