#ifndef DELICIOUS_CONSOLE_H
#define DELICIOUS_CONSOLE_H

#include <vector>
#include <SDL/SDL_Events.h>
#include "console_types.h"
#include "font_renderer.h"
#include "box_renderer.h"

#define CON_BUFFER_SIZE 2048
//#define CON_BUFFER_SIZE 65535
#define CON_INPUT_SIZE 128
#define CON_HISTORY_SIZE 32

class Engine;
class Console {
public:
	bool init(Engine* engine_in);

	void register_variable(const console_var& var);
	float read_variable(cstring name);
	void write_variable(cstring name, float data);

	void key_event(SDL_KeyboardEvent ev);
	void text_event(SDL_TextInputEvent ev);

	void set_font(Font* fnt);
	
	BoxRenderer* get_box_renderer();

	void render();
private:
	char	text_buffer[CON_BUFFER_SIZE];		//Circular buffer
	uint16	front_index;
	uint16	back_index;

	char	input_buffer[CON_INPUT_SIZE];
	uint8	input_index;

	uint16	history_buffer[CON_HISTORY_SIZE];

	int scroll_offset;

	uint8 line_size;
	uint8 visible_lines;
	uint8 border_x;
	uint8 border_y;

	BoxRenderer box_renderer;
	FontRenderer text_renderer;

	std::vector<console_var> variables;

	void write_str(cstring str);
	void write_str(cstring str, uint32 size);
	void write_char(uchar c);
	void buffer_alloc(uint32 size);

	console_var* fetch_var(cstring name);

	void write_to_input(cstring str);
	void execute_input(bool user_input);
	void clear_input();

	Engine* engine;

	void scroll_up();
	void scroll_down();
};

#endif