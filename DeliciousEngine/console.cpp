#include "console.h"

#include <math.h>
#include <fstream>
#include <string>
#include "dcf.h"
#include "dcm.h"
#include "dmath.h"
#include "engine.h"
#include "cvars.h"
#include "cmds.h"

#ifndef self
#define self *this
#endif

/*
Console initialization:										
1. Initialize member variables							
2. Register default console variables					
3. Register default console commands					
4. Load user variable values from config.cfg			
	4a. If the file does not exist, create it.			
5. Open a new logging file to echo console messages to.
*/
bool Console::init(System_Interface sys) {
	write_index = 0;
	line_size = 0;
	border_x = 1;
	border_y = 0;
	scroll_offset = 0;
	read_index = 0;
	input_scroll = 0;

	for (const auto& cvar : default_cvars) {
		register_variable(cvar);
	}
	for (const auto& cmd : default_cmds) {
		register_command(cmd);
	}


	return true;
}

/*
Render the console with a bitmap font and a gui box renderer. The amount of lines
that are rendered is precalculated based upon the font that the console uses.
*/
void Console::render() {
	Screen* scr = systems.screen;
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
	while (*str != '\0') {	//keep printing the string until no more string is left
		buffer_alloc();
		int remaining_line = line_size - (write_index % line_size);
		int remaining_string = dcf::str_len(str);
		if (remaining_string > remaining_line) {
			//buffer_alloc(line_size);
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
		//buffer_alloc(line_size);
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
		dcf::str_fill(text_buffer + write_index, '\0', line_size);
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
	cstring sp = str;
	while (*sp != NULL && input_index != CON_INPUT_LENGTH) {
		input_buffer[input_index++] = *sp++;
	}
}

/*
Attempts to register a variable to the variable list. If one already
exists with the same name, print an error to the console.
*/
void Console::register_variable(const console_var& var) {
	if (fetch_var(var.name)) {
		self << "Register variable: '" << var.name << "' already exists.\n";
	}
	else variables.push_back(var);
}

/*
Attempts to register a command to the command list. If one already exists
with the same name, print an error to the console.
*/
void Console::register_command(const console_cmd& cmd) {
	if (fetch_cmd(cmd.name)) {
		self << "Register command: '" << cmd.name << "' already exists.\n";
	}
	else commands.push_back(cmd);
}

/*
Attempts to return the value of a registered variable. Prints an error
if the variable does not exist.
*/
float Console::read_variable(cstring name) {
	if (console_var* var = fetch_var(name)) {
		return var->value;
	}
	self << "The variable \"" << name << "\" does not exist.\n";
	return 0.0f;
}

/*
Attempts to fetch a pointer of a registered variable. Only returns a
valid pointer if the names match exactly.
*/
console_var* Console::fetch_var(cstring name) {
	for (auto it = variables.begin(); it != variables.end(); it++) {
		if (dcf::str_cmp_exact(name, it->name)) {
			return &(*it);
		}
	}
	return NULL;
}

console_cmd* Console::fetch_cmd(cstring name) {
	for (auto it = commands.begin(); it != commands.end(); it++) {
		if (dcf::str_cmp_exact(name, it->name)) {
			return &(*it);
		}
	}
	return NULL;
}

/*
Attempts to write data to a registered variable. Constrains the assigned value
based upon the type of the registered variable. Prints an error if the registered
variable cannot be edited at runtime or if it does not exist.
*/
void Console::write_variable(cstring name, float value) {
	if (console_var* var = fetch_var(name)) {
		if (var->flags & CVAR_EDIT) {
			switch (var->type) {
			case CVAR_BOOL:  var->value = (value > 0.0f) ? 1.0f : 0.0f; break;
			case CVAR_FLOAT: var->value = value;						break;
			case CVAR_INT:   var->value = static_cast<int>(value);		break;
			}
		}
		else {
			self << "The variable \"" << name << "\" is read-only.\n";
		}
	}
	self << "The variable \"" << name << "\" does not exist.\n";
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
	dcf::str_trim_spaces(input_buffer);
	char* cmd = input_buffer;
	uint argc = dcf::str_count(input_buffer, ' ');
	char* argv = dcf::str_next_word(cmd);
	if (argv != NULL) *(argv - 1) = '\0';


}

/*
Sets the bitmap font that the console renders in. Uses the font size to calculate
how many characters can fit in one line (line_size), how many lines can be rendered
before scrolling (visible_lines) and the aligned extent of the text buffer.
*/
void Console::set_font(Font* fnt) {
	Screen* scr = systems.screen;
	text_renderer.set_font(fnt);

	line_size = (scr->get_width() / fnt->cell_width) - (border_x * 2);
	visible_lines = (scr->get_height() / fnt->cell_height) - (border_y * 2) - 1;
	buffer_extent = CON_BUFFER_SIZE - (CON_BUFFER_SIZE % line_size);
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
void Console::key_event(SDL_KeyboardEvent ev) {
	switch (ev.keysym.sym) {
	case SDLK_BACKSPACE:
		if (input_index == 0) {
			break;
		}
		if (input_buffer[input_index] == NULL || input_index == CON_INPUT_LENGTH) {
			input_index--;
			input_buffer[input_index] = NULL;
		}
		else {
			auto input_size = dcf::str_len(input_buffer);
			input_index--;
			for (uint i = input_index; i < input_size; i++) {
				input_buffer[i] = input_buffer[i + 1];
			}
		}
		scroll_left();
		break;
	case SDLK_TAB:
		//Partial command or variable completion
		break;
	case SDLK_BACKQUOTE:
		//Toggle the console
		break;
	case SDLK_RETURN:
		if (input_buffer[0] == NULL) {
			break;
		}
		//Execute the input found in the input buffer
		write_str(input_buffer, true);
		clear_input();
		input_scroll = 0;
		//scroll_bottom();
		break;
	case SDLK_UP:
		//Cycle back through previously entered commands
		dcf::str_cpy("The quick brown fox jumped over the lazy dogs and then proceeded to eat half a kilo of butter", input_buffer);
		break;
	case SDLK_DOWN:
		dcf::str_cpy("The quick brown fox jumped over the lazy dog and then proceeded to eat half a kilo of butter", input_buffer);
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
	default:
		break;
	}
}

/*
Processes user input that was detected as a text event. Handles inserting
characters in the middle of the buffer, and whether the typed character
can be printed to the screen or not.
*/
void Console::text_event(SDL_TextInputEvent ev) {
	if (dcf::printable(*ev.text) == false || input_index == CON_INPUT_LENGTH) {
		return;
	}
	if (input_insert || input_buffer[input_index] == NULL) {
		//Overwrite the characters over the input cursor
		input_buffer[input_index] = *ev.text;
		input_index++;
	}
	else {
		//Shift the input characters to the right if there is space
		auto input_size = dcf::str_len(input_buffer);
		if (input_size != CON_INPUT_LENGTH) {
			//Loop over the input buffer and shift right
			for (int i = input_size; i != input_index; i--) {
				input_buffer[i] = input_buffer[i - 1];
			}
			input_buffer[input_index] = *ev.text;
			input_index++;
		}
	}
	scroll_right();
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
void Console::clear_buffer() {
	std::memset(text_buffer, '\0', CON_BUFFER_SIZE);
	read_index = 0;
	write_index = 0;
	scroll_offset = 0;
}