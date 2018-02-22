#ifndef DELICIOUS_CONSOLE_H
#define DELICIOUS_CONSOLE_H

#include <vector>
#include <string>
#include <SDL_Events.h>
#include "console_types.h"
#include "font_renderer.h"
#include "box_renderer.h"
#include "input_types.h"

#define CON_BUFFER_SIZE 16384
#define CON_INPUT_SIZE 128
#define CON_INPUT_LENGTH CON_INPUT_SIZE - 1
#define CON_INPUT_SCROLL_MULTIPLE 4
#define CON_HISTORY_SIZE 32

class Console {
public:
	bool init();
	void clean_exit();

	void load_config();
	void update_and_draw();

	void print(cstring format, ...);

	void register_variable(cstring name, system_var* ref, cvar_type type, uint16 access_flags);
	void register_command(cstring name, cmd_callback func);

	system_var* read_variable(cstring name);
	void write_variable(cstring name, int value);
	void write_variable(cstring name, float value);
	void write_variable(cstring name, bool value);
	void write_variable(cstring name, system_var value, cvar_type type);

	void execute_keybind(key_bind* kb);

	void key_input(SDL_KeyboardEvent ev);
	void text_input(SDL_TextInputEvent ev);

	bool is_open();
	void display(bool d);
	void display_toggle();
private:
	std::vector<std::string> report_text;
	bool scroll_to_bottom;

	//Input buffer variables
	char		input_buffer[CON_INPUT_SIZE];
	uint16		input_index;
	uint16		input_scroll;
	bool		input_insert;

	//History & Auto-complete variables
	///uint16	history_buffer[CON_HISTORY_SIZE][CON_INPUT_LENGTH];

	bool display_console;

	//Console variable and command lists
	std::vector<console_var> variables;
	std::vector<console_cmd> commands;

	console_var* find_variable(cstring name);
	console_cmd* find_command(cstring name);

	void set_variable(cstring name, cstring value);
	void set_variable(console_var* cvar, cstring value);

	void execute_string(cstring cmd_str);

	//Input Functions
	void write_to_input(cstring str);
	void execute_input(bool user_input);
	void clear_input();
};
extern Console console;

#endif