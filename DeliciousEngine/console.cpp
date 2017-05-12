#include "console.h"

#include "engine.h"
#include "dcf.h"
#include "dmath.h"

bool Console::init(Engine* engine_in) {
	engine = engine_in;

	write_index = 0;
	back_index = CON_BUFFER_SIZE;

	return true;
}

void Console::write_str(cstring str) { write_str(str, dcf::str_len(str)); }

//
// Main writing function, handles the buffer and any line deletion / wrapping / overflows
//
void Console::write_str(cstring str, uint32 size) {
	buffer_alloc(size);		//Make sure there is enough space for the string to overwrite the buffer contents
	while (*str != NULL) {
		
	}
}

void Console::buffer_alloc(uint32 size) {
	uint16 space = math::delta(back_index, write_index);
	if (size >= space) {
		back_index = (back_index + (size - space)) % CON_BUFFER_SIZE;
		while (text_buffer[back_index] != '\n') {
			back_index = back_index + 1 % CON_BUFFER_SIZE;
		}
	}
}