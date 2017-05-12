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
	uchar text_buffer[CON_BUFFER_SIZE];		//Circular buffer
	uint16 write_index;
	uint16 back_index;

	std::vector<console_var> variables;

	void write_str(cstring str);
	void write_str(cstring str, uint32 size);
	void write_char(uchar c);
	void buffer_alloc(uint32 size);

	Engine* engine;
};

#endif