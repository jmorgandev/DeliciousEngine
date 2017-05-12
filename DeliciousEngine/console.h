#ifndef DELICIOUS_CONSOLE_H
#define DELICIOUS_CONSOLE_H

#include <vector>
#include "console_types.h"

#define CON_BUFFER_SIZE 64

class Engine;
class Console {
public:
	bool init(Engine* engine_in);

private:
	unsigned char text_buffer[CON_BUFFER_SIZE];

	std::vector<console_var> variables;

	Engine* engine;
};

#endif