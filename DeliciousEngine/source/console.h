#ifndef DELICIOUS_CONSOLE_H
#define DELICIOUS_CONSOLE_H

#include <unordered_map>
#include <typeindex>
#include <vector>
#include <string>

#include <SDL_Events.h>

#include "console_types.h"
#include "input_types.h"

#include "module.h"

#define CON_INPUT_SIZE 128
#define CON_INPUT_LENGTH CON_INPUT_SIZE - 1
#define CON_HISTORY_SIZE 32

class Console : public Module {
public:
	using Module::Module;

	bool load() override;
	bool start() override;
	void update() override;
	bool free() override;

	void load_config();
	void update_and_draw();

	void print(std::string str) { report_text.push_back(str); }
	void print(cstring str) { report_text.emplace_back(str); }
	void printf(cstring format, ...);

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

	std::unordered_map<std::string, ConsoleVariable> variables;

	void execute_string(cstring cmd_str);

	//Input Functions
	void write_to_input(cstring str);
	void execute_input(bool user_input);
	void clear_input();
};

#endif