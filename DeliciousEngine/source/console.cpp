#include "console.h"
#include "screen.h"

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "font.h"
#include "dcf.h"
#include "dff.h"
#include "dcm.h"
#include "dmath.h"
#include "cmds.h"

#include <imgui.h>

ConsoleCommand(toggleconsole) {
	console.display_toggle();
}

ConsoleCommand(clear) {
	//@Todo: just clear the vector of strings...
}
ConsoleCommand(quit) {
	//@Todo: Put this command in main.cpp to avoid write_variable overhead
	console.write_variable("eng_running", false);
}

bool Console::init() {
	display_console = false;

	register_command("toggleconsole", cmd_toggleconsole);
	register_command("clear", cmd_clear);
	register_command("quit",  cmd_quit);

	return true;
}

void Console::clean_exit() {
	//@TODO: Write CVars to config file.
	variables.clear();
	commands.clear();
}

static const uint WIN_FLAGS = (ImGuiWindowFlags_NoCollapse|
							   ImGuiWindowFlags_NoSavedSettings);
void Console::update_and_draw() {
	if (display_console) {
		ImGui::SetNextWindowPos(screen.get_imgui_center(), 
								ImGuiCond_Once, 
								ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(screen.get_width() * 0.75, 
										screen.get_height() * 0.75),
								 ImGuiCond_Once);
		ImGui::Begin("Console##console-window", &display_console, WIN_FLAGS);
		const float h = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("##console-report", ImVec2(0, -h), false);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		ImGuiListClipper clipper(report_text.size());
		while (clipper.Step())
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
				ImGui::TextWrapped(report_text[i].c_str());
		if (scroll_to_bottom) {
			ImGui::SetScrollHere();
			scroll_to_bottom = false;
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();
		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("##console-input", input_buffer, CON_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (dcf::str_len(input_buffer) > 0) {
				report_text.push_back(input_buffer);
				dcf::str_cpy("", input_buffer);
				scroll_to_bottom = true;
			}
		}
		ImGui::SetKeyboardFocusHere(-1);
		ImGui::PopItemWidth();
		ImGui::End();
	}
}

#define FORMAT_STR_BUFFER 1024
void Console::print(cstring format, ...) {
	va_list args;
	char buffer[FORMAT_STR_BUFFER];

	va_start(args, format);
	uint length = vsprintf_s(buffer, format, args);
	va_end(args);

	report_text.push_back(buffer);
}

//Copies a string directly to the input buffer
void Console::write_to_input(cstring str) {
	while (*str != NULL && input_index != CON_INPUT_LENGTH) {
		input_buffer[input_index++] = *str++;
	}
}

/*
Attempts to register a variable to the variable list. If one already
exists with the same name, print an error to the console.
*/
void Console::register_variable(cstring name, system_var* ref, cvar_type type, uint16 access_flags) {
	if (!find_variable(name)) {
		assert(dcf::str_len(name) <= CON_MAX_NAME);
		console_var new_cvar = { "", ref, type, access_flags };
		dcf::str_cpy(name, new_cvar.name);
		variables.push_back(new_cvar);
	}
	else print("register_variable: \"%s\" already exists!", name);
}

/*
Attempts to register a command to the command list. If one already exists
with the same name, print an error to the console.
*/
void Console::register_command(cstring name, cmd_callback func) {
	if (!find_command(name)) {
		assert(dcf::str_len(name) <= CON_MAX_NAME);
		console_cmd new_cmd = { "", func };
		dcf::str_cpy(name, new_cmd.name);
		commands.push_back(new_cmd);
	}
	else print("register_command: \"%s\" already exists!", name);
}

/*
Attempts to fetch the pointer of a registered variable. Only returns a
valid pointer if the names match exactly.
*/
console_var* Console::find_variable(cstring name) {
	for (auto itr = variables.begin(); itr != variables.end(); itr++) {
		if (dcf::str_cmp_exact(name, itr->name)) {
			return &(*itr);
		}
	}
	return NULL;
}

/*
Attempts to fetch the pointer of a registered command. Only returns a
valid pointer if the names match exactly.
*/
console_cmd* Console::find_command(cstring name) {
	for (auto itr = commands.begin(); itr != commands.end(); itr++) {
		if (dcf::str_cmp_exact(name, itr->name)) {
			return &(*itr);
		}
	}
	return NULL;
}

system_var* Console::read_variable(cstring name) {
	if (console_var* cvar = find_variable(name)) {
		return cvar->value;
	}
	print("read_variable: \"%\" does not exist!", name);
	return nullptr;
}

void Console::write_variable(cstring name, int value) {
	write_variable(name, value, CVAR_INT);
}
void Console::write_variable(cstring name, float value) {
	write_variable(name, value, CVAR_FLOAT);
}
void Console::write_variable(cstring name, bool value) {
	write_variable(name, value, CVAR_BOOL);
}
void Console::write_variable(cstring name, system_var value, cvar_type type) {
	if (console_var* cvar = find_variable(name)) {
		if (cvar->type == type) {
			*cvar->value = value;
		}
		else {
			print("write_variable: \"%s\" type mismatch!", name);
		}
	} 
	else {
		print("write_variable: \"%s\" does not exist!", name);
	}
}

/*
Separates the input buffer into tokens and attempts to execute the corresponding
command / variable assignment. Echos the input to the message box if it was entered
by the user (With the return key).
*/
void Console::execute_input(bool user_input) {
	if (user_input) {
		print(input_buffer);
	}
	execute_string(input_buffer);
	clear_input();
}

void Console::execute_keybind(key_bind* kb) {
	if (display_console == false || strcmp(kb->command, "toggleconsole")) {
		execute_string(kb->command);
	}
}

void Console::execute_string(cstring cmd_str) {
	if (dcf::is_glyph(*cmd_str) == false) {
		cmd_str = dcf::str_next_glyph(cmd_str);
		if (cmd_str == nullptr) return;
	}

	char buffer[CON_INPUT_SIZE];
	dcf::str_cpy(cmd_str, buffer);
	dcf::str_trim_spaces(buffer);

	char* label = &buffer[0];
	std::vector<cstring> args;

	char* arg_str = dcf::str_next_word(label);
	if (arg_str != nullptr) {
		*(arg_str - 1) = '\0';
		dcf::str_split_vector(arg_str, args, ' ');
	}

	if (console_var* cvar = find_variable(label)) {
		if (args.empty()) {
			switch (cvar->type) {
			case CVAR_INT:
				print("%s is %i", cvar->name, cvar->value->as_int);
				break;
			case CVAR_FLOAT:
				print("%s is %f", cvar->name, cvar->value->as_float);
				break;
			case CVAR_BOOL:
				print("%s is %i", cvar->name, cvar->value->as_bool);
				break;
			}
		}
		else if (args.size() == 1) set_variable(cvar, args[0]);
		else print("Set variable usage: <var> <value>");
	}
	else if (console_cmd* cmd = find_command(label)) cmd->callback(args);
	else print("Unknown command/variable: \"%s\"", label);
}

/*
Processes user input that was not detected as a text event. Handles scrolling,
erasing, executing, auto-completion, input history, and toggling the console.
*/
void Console::key_input(SDL_KeyboardEvent ev) {
	switch (ev.keysym.sym) {
	case SDLK_BACKSPACE:
		if (input_index != 0) {
			if (input_buffer[input_index] == '\0' || input_index == CON_INPUT_LENGTH) {
				input_index--;
				input_buffer[input_index] = '\0';
			}
			else {
				input_index--;
				dcf::str_shift_left(input_buffer, input_index);
			}
		}
		break;
	case SDLK_DELETE:
		if (input_index != CON_INPUT_LENGTH && input_buffer[input_index] + 1 != '\0') {
			dcf::str_shift_left(input_buffer, input_index);
		}
		break;
	case SDLK_TAB:
		//Partial command or variable completion
		break;
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		if (input_buffer[0] == NULL) {
			break;
		}
		//Execute the string in the input buffer
		execute_input(true);
		input_scroll = 0;
		break;
	case SDLK_UP:
		//Cycle back through previously entered commands#
		write_to_input("odaijn;odna;odwdioa;dowoinda;o");
		break;
	case SDLK_DOWN:
		//Cycle forward through previously entered commands
		break;
	case SDLK_LEFT:
		//Shift the input cursor to the left
		if (input_index != 0) {
			input_index--;
		}
		break;
	case SDLK_RIGHT:
		//Shift the input cursor to the right
		if (input_buffer[input_index] != NULL && input_index != CON_INPUT_LENGTH) {
			input_index++;
		}
		break;
	case SDLK_PAGEUP:
		break;
	case SDLK_PAGEDOWN:
		break;
	case SDLK_INSERT:
		//Toggle insertion mode
		input_insert = !input_insert;
		break;
	}
}

/*
Processes user input that was detected as a text event. Handles inserting
characters in the middle of the buffer, and whether the typed character
can be printed to the screen or not.
*/
void Console::text_input(SDL_TextInputEvent ev) {
	if (dcf::is_printable(*ev.text) && input_index != CON_INPUT_LENGTH) {
		if (input_insert == false && input_buffer[input_index] != '\0') {
			uint input_size = dcf::str_len(input_buffer);
			if (input_size != CON_INPUT_LENGTH) {
				dcf::str_shift_right(input_buffer, input_index);
				input_buffer[input_index++] = *ev.text;
			}
		}
		else {
			input_buffer[input_index++] = *ev.text;
		}
	}
}

/*
Wipes the input buffer with null characters and resets the input index.
*/
void Console::clear_input() {
	for (int i = 0; i < CON_INPUT_LENGTH; i++) {
		input_buffer[i] = NULL;
	}
	input_index = 0;
}

bool Console::is_open() {
	return display_console;
}

void Console::load_config() {
	std::fstream config_file("config.cfg", std::fstream::in);
	if (config_file.is_open()) {
		std::string line;
		while (std::getline(config_file, line)) {
			dcf::str_cpy(line.c_str(), input_buffer);
			//dcf::str_trim_spaces(input_buffer);
			char* label = input_buffer;
			char* value = dcf::str_next_word(label);
			if (value != NULL) {
				*dcf::str_find(label, ' ') = '\0';
				//set_variable(label, value, true);
			}
		}
		clear_input();
	}
	else {
		config_file.open("config.cfg", std::fstream::out);
		for (const auto& cvar : variables) {
			if (cvar.flags & CVAR_CONFIG) {
				config_file << cvar.name << " ";
				switch (cvar.type) {
				//case CVAR_BOOL: config_file << cvar.value.as_bool; break;
				//case CVAR_INT: config_file << cvar.value.as_int; break;
				//case CVAR_FLOAT: config_file << cvar.value.as_float; break;
				}
				config_file << '\n';
			}
		}
	}
	config_file.close();
}

//void Console::load_config() {
//	std::fstream config_file("config.cfg", std::fstream::in);
//	if (config_file.is_open()) {
//		std::string line;
//		while (std::getline(config_file, line)) {
//			dcf::str_cpy(line.c_str(), input_buffer);
//			dcf::str_trim_spaces(input_buffer);
//			
//			char* label = input_buffer;
//			char* value = dcf::str_next_word(label);
//			if (value != NULL) {
//				*dcf::str_find(label, ' ') = '\0';
//				set_variable(label, value);
//			}
//			clear_input();
//		}
//	}
//	else {
//		config_file.open("config.cfg", std::fstream::out);
//		for (const auto& cvar : variables) {
//			if (cvar.flags & CVAR_CONFIG) {
//				config_file << cvar.name;
//				switch (cvar.type) {
//				case CVAR_INT: config_file << cvar.value->as_int; break;
//				case CVAR_FLOAT: config_file << cvar.value->as_float; break;
//				case CVAR_BOOL: config_file << cvar.value->as_bool; break;
//				}
//				config_file << '\n';
//			}
//		}
//	}
//	config_file.close();
//}

void Console::set_variable(cstring name, cstring value) {
	if (console_var* cvar = find_variable(name)) set_variable(cvar, value);
	else print("set_variable: \"%s\" does not exist!", name);
}

void Console::set_variable(console_var* cvar, cstring value) {
	if (cvar->flags & CVAR_MUTABLE) {
		switch (cvar->type) {
		case CVAR_INT: 
			cvar->value->as_int = std::atoi(value); 
			break;
		case CVAR_FLOAT: 
			cvar->value->as_float = (float)std::atof(value);
			break;
		case CVAR_BOOL:
			if (value[0] == 't' || value[0] == 'T' || std::atoi(value) > 0) cvar->value->as_bool = true;
			else cvar->value->as_bool = false;
			break;
		}
	}
	else print("%s is read-only.", cvar->name);
}

void Console::display(bool d) {
	display_console = d;
}

void Console::display_toggle() {
	display_console = !display_console;
}