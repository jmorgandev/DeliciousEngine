#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include "console.h"
#include "screen.h"

class Engine {
public:
	bool init(char** argv, int argc);
	void run();
	void clean();

	Console* get_console() { return &console; }
	Screen* get_screen() { return &screen; }

private:
	bool running;

	Console	console;
	Screen	screen;
};

#endif