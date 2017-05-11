#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include "console.h"

class Engine {
public:
	bool init(char** argv, int argc);
	void run();
	void clean();

	Console* get_console() { return &console; }

private:
	bool running;

	Console	console;
};

#endif