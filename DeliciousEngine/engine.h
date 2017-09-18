#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include "console.h"
#include "screen.h"
#include "resources.h"
#include "input.h"

class Engine {
public:
	bool init(char** argv, int argc);
	void run();
	void clean();

private:
	bool running;

	Console		console;
	Resources	resources;
	Screen		screen;
	Input		input;

	void flush_events();
};

#endif