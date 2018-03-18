#ifndef DELICIOUS_CONSOLE_H
#define DELICIOUS_CONSOLE_H

#include <vector>
#include <string>

#include <SDL_Events.h>

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

	void print(std::string str) { report_text.push_back(str); }
	void print(cstring str) { report_text.emplace_back(str); }
	void printf(cstring format, ...);

	void register_command(cstring name, CmdFunc callback);
	void register_variable(cstring name, SystemVar* ptr, CvarType t, uint16 access_flags);

	SystemVar* get_variable(cstring name);
	void set_variable(cstring name, int i) {
		write_variable(name, i, CVAR_INT);
	}
	void set_variable(cstring name, float f) {
		write_variable(name, f, CVAR_FLOAT);
	}
	void set_variable(cstring name, bool b) {
		write_variable(name, b, CVAR_BOOL);
	}
	void write_variable(cstring name, SystemVar var, CvarType t);

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
	std::vector<ConsoleVar> variables;
	std::vector<ConsoleCmd> commands;

	ConsoleVar* find_variable(cstring name);
	ConsoleCmd* find_command(cstring name);

	//void set_variable(cstring name, cstring value);
	//void set_variable(ConsoleVar* cvar, cstring value);
	void assign_variable(cstring name, cstring value);
	void assign_variable(ConsoleVar* cvar, cstring value);

	void execute_string(cstring cmd_str);

	//Input Functions
	void write_to_input(cstring str);
	void execute_input(bool user_input);
	void clear_input();
};
extern Console console;

#endif