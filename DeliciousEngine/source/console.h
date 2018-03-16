#ifndef DELICIOUS_CONSOLE_H
#define DELICIOUS_CONSOLE_H

#include <vector>
#include <string>

#include <SDL_Events.h>

#include <sol.hpp>

#include "console_types.h"
#include "input_types.h"

#define CON_INPUT_SIZE 128
#define CON_INPUT_LENGTH CON_INPUT_SIZE - 1
#define CON_HISTORY_SIZE 32

class Console {
public:
	bool init();
	void clean_exit();

	void load_config();
	void update_and_draw();

	void print(cstring str) { report_text.push_back(str); }
	void printf(cstring format, ...);

	void register_variable(cstring name, system_var* ref, cvar_type type, uint16 access_flags);
	void register_command(cstring name, cmd_callback func);
	void register_lua_command(cstring name, sol::function func);

	system_var* read_variable(cstring name);
	void write_variable(cstring name, int value);
	void write_variable(cstring name, float value);
	void write_variable(cstring name, bool value);
	void write_variable(cstring name, system_var value, cvar_type type);

	void execute_keybind(key_bind* kb);

	bool is_open();
	void display(bool d);
	void display_toggle();
private:
	std::vector<std::string> report_text;
	char input_buffer[CON_INPUT_SIZE];
	bool scroll_to_bottom;

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