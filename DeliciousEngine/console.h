#ifndef DELICIOUS_CONSOLE_H
#define DELICIOUS_CONSOLE_H

#include <vector>
#include "console_types.h"

#define CON_BUFFER_SIZE 64

class Engine;
class Console {
public:
	bool init(Engine* engine_in);

	void register_variable(const console_var& var);
	float read_variable(cstring name);
	void write_variable(cstring name, float data);

private:
	char text_buffer[CON_BUFFER_SIZE];		//Circular buffer
	uint16 front_index;
	uint16 back_index;

	uint8 line_size;

	std::vector<console_var> variables;

	void write_str(cstring str);
	void write_str(cstring str, uint32 size);
	void write_char(uchar c);
	void buffer_alloc(uint32 size);

	console_var* fetch_var(cstring name);

	Engine* engine;
};

#endif