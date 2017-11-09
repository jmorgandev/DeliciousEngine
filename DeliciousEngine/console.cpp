#include "console.h"

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <typeinfo>
#include "dcf.h"
#include "dff.h"
#include "dcm.h"
#include "dmath.h"
#include "engine.h"
#include "cmds.h"

/*
Console initialization:										
1. Initialize member variables											
2. Register default console commands							
3. Open a new logging file to echo console messages to.
*/
bool Console::init(System_Ref sys) {
	system = sys;

	write_index = 0;
	line_size = 0;
	border_x = 1;
	border_y = 0;
	scroll_offset = 0;
	read_index = 0;
	input_scroll = 0;
	display_console = false;

	for (const auto& cmd : default_cmds) {
		register_command(cmd);
	}

	clear();

	return true;
}

void Console::terminate() {
	//@TODO: Write CVars to config file.

	variables.clear();
	commands.clear();
}

/*
Render the console with a bitmap font and a gui box renderer. The amount of lines
that are rendered is precalculated based upon the font that the console uses.
*/
void Console::render() {
	if (display_console == false) {
		return;
	}

	Screen* scr = system.screen;
	Font* fnt = text_renderer.get_font();

	box_renderer.begin(scr->get_width(), scr->get_height());
	box_renderer.draw(0, 0, scr->get_width(), fnt->cell_height * visible_lines, glm::vec4(0.7f, 0.5f, 1.0f, 0.2f));
	box_renderer.draw(0, (fnt->cell_height * visible_lines), scr->get_width(), (fnt->cell_height * visible_lines) + fnt->cell_height, glm::vec4(0.7f, 0.5f, 1.0f, 0.5f));

	box_renderer.draw(
		((input_index - input_scroll) * fnt->cell_width) + fnt->cell_width,
		(fnt->cell_height * visible_lines) + 2,
		(((input_index - input_scroll) * fnt->cell_width) + fnt->cell_width) + 3,
		((fnt->cell_height * visible_lines) + fnt->cell_height) - 2,
		glm::vec4(1.0f, 1.0f, 1.0f, 0.8f)
	);

	box_renderer.end();

	text_renderer.begin(scr->get_width(), scr->get_height());

	//Draw message box
	int x_cursor = 0, y_cursor = 0;
	int render_start = (read_index + scroll_offset) % buffer_extent;
	for (int i = render_start; i != write_index; i = (i + 1) % buffer_extent) {
		if (y_cursor == visible_lines) {
			break;
		}
		if (dcf::is_glyph(text_buffer[i])) {
			text_renderer.draw_char(text_buffer[i], (x_cursor + border_x) * fnt->cell_width, ((y_cursor + border_y) * fnt->cell_height));
		}
		x_cursor++;
		if (x_cursor == line_size) {
			x_cursor = 0;
			y_cursor++;
		}
	}

	//Draw Input Box
	for (int i = 0; i < line_size && i < CON_INPUT_LENGTH; i++) {
		char c = input_buffer[i + input_scroll];
		if (dcf::is_glyph(c)) {
			text_renderer.draw_char(c, (i + border_x) * fnt->cell_width, fnt->cell_height * visible_lines);
		}
		else if (c == '\0') {
			break;
		}
	}

	text_renderer.end();
}

/*
Checks the size of the string to write before passing it to the
overloaded function.
*/
void Console::write_str(cstring str, bool new_line) { 
	write_str(str, dcf::str_len(str), new_line);
}

/*
Main string printing function, makes necessary word wrapping and
circular buffer allocations to print the passed in string.
"new_line" is used to end this printing call with a new line.
*/
void Console::write_str(cstring str, uint32 size, bool new_line) {
	if (text_renderer.get_font() == nullptr) {
		//@TODO: Handle non graphical printing.
		return;
	}
	while (*str != '\0') {	//keep printing the string until no more string is left
		buffer_alloc();
		int remaining_line = line_size - (write_index % line_size);
		int remaining_string = dcf::str_len(str);
		if (remaining_string > remaining_line) {
			//initially try to wrap from the character that causes the overflow
			cstring wrap_point = str + line_size;
			if (dcf::is_wspace(*wrap_point) == false) {
				//check if we can wrap from a newline or space character
				cstring newline_wrap = dcf::str_prev_instance(wrap_point, str, '\n');
				cstring space_wrap = dcf::str_prev_instance(wrap_point, str, ' ');
				if (newline_wrap) {
					wrap_point = newline_wrap;
				}
				else if (space_wrap) {
					wrap_point = space_wrap;
				}
			}
			//print string up to the wrap point
			while (str != wrap_point) {
				write_char(*str++);
			}
			if (dcf::is_wspace(*str)) {
				str++;
			}
			terminate_current_line();
		}
		else {
			//just print the string as normal
			while (*str != '\0') {
				if (*str == '\n') {
					terminate_current_line();
					str++;
				}
				else {
					write_char(*str++);
				}
			}
			break;
		}
	}
	//if we need to move to the next line...
	if (new_line && write_index % line_size != 0) {
		terminate_current_line();
		buffer_alloc();
	}
}

/*
Puts a character in the text buffer and advances
the write index. Keeps the write index within range of
the buffer extent
*/
void Console::write_char(uchar c) {
	text_buffer[write_index] = c;
	write_index = ++write_index % buffer_extent;
}

void Console::buffer_alloc() {
	if (write_index == read_index && text_buffer[read_index] != '\0') {
		read_index = (read_index + line_size) % buffer_extent;
		std::memset(text_buffer + write_index, '\0', line_size);
		//dcf::str_fill(text_buffer + write_index, '\0', line_size);
	}
}

/*
Fills the rest of the current line (determined with the write_index) with
null characters in order to start printing at the start of the next line
*/
void Console::terminate_current_line() {
	while (write_index % line_size != 0) {
		write_char('\0');
	}
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
	if (find_variable(name)) {
		self << "register_variable: '" << name << "' already exists!\n";
	}
	else {
		assert(dcf::str_len(name) <= CON_MAX_NAME);
		console_var new_cvar = { "", ref, type, access_flags };
		dcf::str_cpy(name, new_cvar.name);
		variables.push_back(new_cvar);
	}
}

/*
Attempts to register a command to the command list. If one already exists
with the same name, print an error to the console.
*/
void Console::register_command(const console_cmd& cmd) {
	if (find_command(cmd.name)) {
		self << "register_command: '" << cmd.name << "' already exists!\n";
	}
	else commands.push_back(cmd);
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
	self << "read_variable: '" << name << "' does not exist!\n";
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
			self << "write_variable: \"" << name << "\" type mismatch!\n";
		}
	} 
	else {
		self << "write_variable: \"" << name << "\" does not exist!\n";
	}
}

/*
Separates the input buffer into tokens and attempts to execute the corresponding
command / variable assignment. Echos the input to the message box if it was entered
by the user (With the return key).
*/
void Console::execute_input(bool user_input) {
	if (user_input) {
		write_str(input_buffer, true);
	}
	execute_string(input_buffer);
	clear_input();
}

void Console::execute_keybind(key_bind* kb) {
	if (dcf::str_cmp_exact(kb->command, "toggleconsole")) {
		if (toggle_key != kb->keycode) {
			toggle_key = kb->keycode;
		}
		execute_string(kb->command);
	}
	else if (display_console == false) {
		execute_string(kb->command);
	}
}

void Console::execute_string(cstring cmd_str) {
	if (dcf::is_glyph(*cmd_str) == false) {
		cmd_str = dcf::str_next_glyph(cmd_str);
		if (cmd_str == nullptr) {
			return;
		}
	}
	char buffer[CON_INPUT_SIZE];
	dcf::str_cpy(cmd_str, buffer);
	dcf::str_trim_spaces(buffer);

	char* label = nullptr;
	if (dcf::is_glyph(buffer[0])) {
		label = buffer;
	}
	else {
		label = dcf::str_next_glyph(buffer);
		if (label == nullptr) {
			return;
		}
	}

	uint argc = 0;
	char* argv = dcf::str_next_word(label);
	if (argv != nullptr) {
		*dcf::str_find(label, ' ') = '\0';
		char* sp = argv;
		while (sp != nullptr && *sp != '\0') {
			argc++;
			sp = dcf::str_next_word(sp);
		}
	}

	if (console_var* cvar = find_variable(label)) {
		if (argc == 1) {
			//set_variable(cvar, argv);
		}
		else {
			self << "Set variable usage: <var> <value>\n";
		}
	}
	else if (console_cmd* cmd = find_command(label)) {
		cmd->callback(system, argv, argc);
	}
	else {
		self << "Unknown command/variable: '" << label << "'\n";
	}
}

/*
Sets the bitmap font that the console renders in. Uses the font size to calculate
how many characters can fit in one line (line_size), how many lines can be rendered
before scrolling (visible_lines), and the aligned extent of the text buffer.
*/
void Console::set_font(Font* fnt) {
	text_renderer.set_font(fnt);
	display_reformat();
}

//OPERATOR OVERLOADS
Console& Console::operator<<(const bool& rhs) {
	if (rhs == true) write_str("true");
	else write_str("false");
	return *this;
}
Console& Console::operator<<(const char& rhs) {
	char temp_str[2] = {rhs, '\0'};
	write_str(temp_str);
	return *this;
}
Console& Console::operator<<(const int& rhs) {
	std::string str = std::to_string(rhs);
	write_str(str.c_str());
	return *this;
}
Console& Console::operator<<(const float& rhs) {
	std::string str = std::to_string(rhs);
	write_str(str.c_str());
	return *this;
}
Console& Console::operator<<(const double& rhs) {
	std::string str = std::to_string(rhs);
	write_str(str.c_str());
	return *this;
}
Console & Console::operator<<(cstring rhs) {
	write_str(rhs);
	return *this;
}
Console & Console::operator<<(const std::string & rhs) {
	write_str(rhs.c_str());
	return *this;
}

/*
Processes user input that was not detected as a text event. Handles scrolling,
erasing, executing, auto-completion, input history, and toggling the console.
*/
void Console::key_input(SDL_KeyboardEvent ev) {
	if (ev.keysym.sym != toggle_key) {
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
				scroll_left();
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
				scroll_left();
			}
			break;
		case SDLK_RIGHT:
			//Shift the input cursor to the right
			if (input_buffer[input_index] != NULL && input_index != CON_INPUT_LENGTH) {
				input_index++;
				scroll_right();
			}
			break;
		case SDLK_PAGEUP:
			scroll_up();
			break;
		case SDLK_PAGEDOWN:
			scroll_down();
			break;
		case SDLK_INSERT:
			//Toggle insertion mode
			input_insert = !input_insert;
			break;
		}
	}
	last_input = ev.keysym.sym;
}

/*
Processes user input that was detected as a text event. Handles inserting
characters in the middle of the buffer, and whether the typed character
can be printed to the screen or not.
*/
void Console::text_input(SDL_TextInputEvent ev) {
	if (last_input != toggle_key && dcf::printable(*ev.text) && input_index != CON_INPUT_LENGTH) {
		if (input_insert == false && input_buffer[input_index] != '\0') {
			uint input_size = dcf::str_len(input_buffer);
			if (input_size != CON_INPUT_LENGTH) {
				dcf::str_shift_right(input_buffer, input_index);
				input_buffer[input_index++] = *ev.text;
				scroll_right();
			}
		}
		else {
			input_buffer[input_index++] = *ev.text;
			scroll_right();
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

/*
Scrolls the message box up by one line unless the scroll offset is zero.
Returns true if the scroll was successful.
*/
bool Console::scroll_up() {
	if (scroll_offset == 0) {
		return false;
	}
	scroll_offset -= line_size;
	return true;
}

/*
Scrolls the message box down by one line
*/
bool Console::scroll_down() {
	int scroll_edge = (scroll_offset + (line_size * visible_lines)) % buffer_extent;
	int visible_edge = (buffer_extent - line_size) - (line_size * visible_lines);
	if (text_buffer[scroll_edge] == '\0' || scroll_offset == visible_edge) {
		return false;
	}
	scroll_offset += line_size;
	return true;
}

/*
Continues to scroll the message box up until it cannot anymore
*/
void Console::scroll_top() {
	while (scroll_up() == true);
}

/*
Continues to scroll the message box down until it cannot anymore
*/
void Console::scroll_bottom() {
	while (scroll_down() == true);
}

/*
Scrolls the input box left by a multiple of the line size
*/
bool Console::scroll_left() {
	if (input_index - input_scroll < 0) {
		if (input_scroll - (line_size / CON_INPUT_SCROLL_MULTIPLE) < 0) {
			input_scroll = 0;
		}
		else input_scroll -= line_size / CON_INPUT_SCROLL_MULTIPLE;
		return true;
	}
	return false;
}

/*
Scrolls the input box right by a multiple of the line size
*/
bool Console::scroll_right() {
	if (input_index - input_scroll == line_size) {
		input_scroll += line_size / CON_INPUT_SCROLL_MULTIPLE;
		if (input_scroll > CON_INPUT_LENGTH - line_size) {
			input_scroll = CON_INPUT_LENGTH - line_size;
		}
		return true;
	}
	return false;
}

/*
Sets up the GUI rendering properties of the Console
*/
void Console::set_gui_properties(GLuint vao, Shader* shader) {
	box_renderer.set_vao(vao);
	box_renderer.set_shader(shader);
}

/*
Wipe the message box by filling it with null characters. Also reset the readwrite indexes and the
scroll offset.
*/
void Console::clear() {
	std::memset(text_buffer, '\0', CON_BUFFER_SIZE);
	read_index = 0;
	write_index = 0;
	scroll_offset = 0;
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
	if (console_var* cvar = find_variable(name)) {
		set_variable(cvar, value);
	}
	else {
		self << "set_variable: '" << name << "' does not exist!\n";
	}
}

void Console::set_variable(console_var* cvar, cstring value) {
	switch (cvar->type) {
	case CVAR_INT: cvar->value->as_int = std::atoi(value); break;
	case CVAR_FLOAT: cvar->value->as_float = std::atof(value); break;
	case CVAR_BOOL: cvar->value->as_bool = (value[0] == '1') ? true : false; break;
	}
}

void Console::display(bool d) {
	display_console = d;
}

void Console::display_toggle() {
	display_console = !display_console;
}

void Console::display_reformat() {
	clear();

	Screen* scr = system.screen;
	Font* fnt = text_renderer.get_font();

	line_size = (scr->get_width() / fnt->cell_width) - (border_x * 2);
	visible_lines = (scr->get_height() / fnt->cell_height) - (border_y * 2) - 1;
	buffer_extent = CON_BUFFER_SIZE - (CON_BUFFER_SIZE % line_size);

	
}