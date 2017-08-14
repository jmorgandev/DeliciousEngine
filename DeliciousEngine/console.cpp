#include "console.h"

#include <math.h>
#include <fstream>
#include <string>
#include "dcf.h"
#include "dmath.h"
#include "engine.h"
#include "std_cvars.h"

bool Console::init(Engine* engine_in) {
	engine = engine_in;

	front_index = 0;
	line_size = 0;
	border_x = 1;
	border_y = 0;
	scroll_offset = 0;
	back_index = CON_BUFFER_SIZE;

	for (const auto& cvar : standard_cvars) {
		register_variable(cvar);
	}
	/*
	std::fstream config_file("config.cfg", std::fstream::in);
	if (config_file.is_open()) {
		std::string line_in;
		while (std::getline(config_file, line_in)) {
			write_to_input(line_in.c_str());
			execute_input(false);
		}
		return true;
	}
	else {
		config_file.open("config.cfg", std::fstream::out);
		if (!config_file.is_open()) return false;

		for (const auto& cvar : variables) {
			if (cvar.flags & CVAR_WRITE) {
				config_file << cvar.name << " ";
				switch (cvar.type) {
				case VAR_INT: config_file << (int)cvar.data; break;
				case VAR_BOOL: config_file << (cvar.data < 1.0f) ? false : true; break;
				case VAR_FLOAT: config_file << cvar.data; break;
				}
				config_file << "\n";
			}
		}
		return true;
	}
	*/
	return true;
}

void Console::write_str(cstring str) { 
	write_str(str, dcf::str_len(str));
}
//
// Main writing function, handles the buffer and any line deletion / wrapping / overflows
// @TODO - Tidy this function up
//
void Console::write_str(cstring str, uint32 size) {
	buffer_alloc(size);		//Make sure there is enough space for the string to overwrite the buffer contents
	while (*str != NULL) {	//Keep looping until no more words left in string

		int str_remaining = dcf::str_len(str);

		if (str_remaining > line_size) {		//Check if wrapping needs to occur
			cstring wrap_point = str + line_size;
			if (str[line_size] == ' ') {
				while (str != wrap_point) {
					write_char(*str++);
				}
				str++;
			}
			else {
				wrap_point = dcf::str_prev_instance(wrap_point, str, ' ');
				if (wrap_point == NULL) {
					//No space was found, just let the text wrap itself.
					for (int i = 0; i < line_size; i++) write_char(*str++);
				}
				else {
					while (str != wrap_point) {
						write_char(*str++);
					}
					//Fill rest of line with null characters
					for (int i = 0; i < str_remaining - line_size; i++) write_char('\0');
					*str++;
				}
			}
		}
		else {	//If no wrapping needs to occur, print string as normal.
			for (; *str != NULL; str++) write_char(*str);
		}
	}
}
void Console::write_char(uchar c) {
	text_buffer[front_index] = c;
	front_index = ++front_index % CON_BUFFER_SIZE;
}

void Console::buffer_alloc(uint32 size) {
	uint16 space = math::delta(back_index, front_index);
	if (size >= space) {
		back_index = (back_index + (size - space)) % CON_BUFFER_SIZE;
		while (text_buffer[back_index] != '\0') {
			back_index = (back_index + 1) % CON_BUFFER_SIZE;
		}
		back_index++;
	}
}

void Console::register_variable(const console_var& var) {
	if (fetch_var(var.name)) {
		//Print an error log to console about already existing variable...
		return;
	}
	variables.push_back(var);
}

console_var* Console::fetch_var(cstring name) {
	for (auto it = variables.begin(); it != variables.end(); it++) {
		if (dcf::str_cmp_exact(name, it->name)) {
			return &(*it);
		}
	}
	return NULL;
}

float Console::read_variable(cstring name) {
	if (console_var* var = fetch_var(name)) {
		return var->data;
	}
	//Error: Variable does not exist...
}

//
// Format the variable depending upon it's type whenever it is modified.
//
void Console::write_variable(cstring name, float data) {
	if (console_var* var = fetch_var(name)) {
		if (var->flags & CVAR_EDIT) {
			switch (var->type) {
			case VAR_BOOL:  var->data = (data > 0.0f) ? 1.0f : 0.0f; break;
			case VAR_INT:   var->data = truncf(data);
			case VAR_FLOAT: var->data = data;
			}
		}
		else {
			//Error: variable is read-only
		}
	}
	//Error: variable does not exist.
}

void Console::write_to_input(cstring str) {
	cstring sp = str;
	while (*sp != NULL && input_index != CON_INPUT_SIZE) {
		input_buffer[input_index++] = *sp++;
	}
}

void Console::execute_input(bool user_input) {
	if (user_input) {
		write_str(input_buffer, dcf::str_len(input_buffer) + 1);
		write_char('\n');
	}
	
}

void Console::set_font(Font* fnt) {
	text_renderer.set_font(fnt);

	line_size = (engine->get_screen()->get_width() / fnt->cell_width) - (border_x * 2);
	visible_lines = (engine->get_screen()->get_height() / fnt->cell_height) - (border_y * 2) - 1;

	cstring txts[] = {
		"Jamie Morgan\n",
		"Hello World!\n",
		"Is anyone out there?\n",
		"Seriously say something\n"
	};
	for (int i = 0; i < visible_lines - 3; i++) {
		write_str(txts[i % 4]);
	}
}

void Console::render() {
	Screen* scr = engine->get_screen();
	Font* fnt = text_renderer.get_font();

	box_renderer.begin(scr->get_width(), scr->get_height());
	box_renderer.draw(0, 0, scr->get_width(), fnt->cell_height * visible_lines, glm::vec4(0.7f, 0.5f, 1.0f, 0.2f));
	box_renderer.draw(0, (fnt->cell_height * visible_lines), scr->get_width(), (fnt->cell_height * visible_lines) + fnt->cell_height, glm::vec4(0.7f, 0.5f, 1.0f, 0.5f));
	
	box_renderer.draw(
		(input_index * fnt->cell_width) + fnt->cell_width,
		(fnt->cell_height * visible_lines) + 2,
		((input_index * fnt->cell_width) + fnt->cell_width) + 3,
		((fnt->cell_height * visible_lines) + fnt->cell_height) - 2,
		glm::vec4(1.0f, 1.0f, 1.0f, 0.8f)
	);

	box_renderer.end();

	text_renderer.begin(scr->get_width(), scr->get_height());

	//Draw Message Box
	//@TODO - Make a robust rendering function
	int x_cursor = 0, y_cursor = 0;
	for (int i = (back_index % CON_BUFFER_SIZE) + scroll_offset; i != front_index; i = (i + 1) % CON_BUFFER_SIZE) {
		if (y_cursor == visible_lines) break;
		if (text_buffer[i] == '\n') {
			x_cursor = 0;
			y_cursor++;
			continue;
		}
		if (text_buffer[i] == '\0') {
			break;
		}
		text_renderer.draw_char(text_buffer[i], (x_cursor + border_x) * fnt->cell_width, ((y_cursor + border_y) * fnt->cell_height));
		x_cursor++;
		if (x_cursor == line_size) {
			x_cursor = 0;
			y_cursor++;
		}
	}

	//Draw Input Box
	for (int i = 0; i < CON_INPUT_SIZE; i++) {
		if (input_buffer[i] == '\0') {
			break;
		}
		text_renderer.draw_char(input_buffer[i], (i + border_x) * fnt->cell_width, fnt->cell_height * visible_lines);
	}

	text_renderer.end();
}

BoxRenderer* Console::get_box_renderer() {
	return &box_renderer;
}

void Console::key_event(SDL_KeyboardEvent ev) {
	switch (ev.keysym.sym) {
	case SDLK_BACKSPACE:
		if (input_index == 0) {
			break;
		}
		input_index--;
		input_buffer[input_index] = NULL;
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
		input_buffer[input_index] = '\n';
		write_str(input_buffer);
		clear_input();
		break;
	case SDLK_UP:
		//Cycle back through previously entered commands
		break;
	case SDLK_DOWN:
		//Cycle forward through previously entered commands
		break;
	case SDLK_PAGEUP:
		//line_scroll++;
		//if (line_scroll > (CON_BUFFER_SIZE / 2)) line_scroll = CON_BUFFER_SIZE / 2;
		scroll_up();
		break;
	case SDLK_PAGEDOWN:
		//line_scroll--;
		//if (line_scroll < 0) line_scroll = 0;
		scroll_down();
		break;
	default:
		break;
		//if (dcf::printable(input.sym) == false || input_index == CON_INPUT_SIZE) {
		//	break;
		//}
		//input_buffer[input_index++] = 
		//break;
	}
}

void Console::text_event(SDL_TextInputEvent ev) {
	if (dcf::printable(*ev.text) == false || input_index == CON_INPUT_SIZE) {
		return;
	}
	input_buffer[input_index] = *ev.text;
	input_index++;
}

void Console::clear_input() {
	for (int i = 0; i < CON_INPUT_SIZE; i++) {
		input_buffer[i] = NULL;
	}
	input_index = 0;
}

void Console::scroll_up() {
	if (scroll_offset == 0) return;
	int new_offset = 0;
	int count = 0;
	for (int i = 1; i < line_size; i++) {
		new_offset = scroll_offset - i;
		if (text_buffer[new_offset] == '\n') {
			count++;
			if (count == 2) {
				new_offset++;
				break;
			}
		}
	}
	scroll_offset = new_offset;
	if (scroll_offset < 0) scroll_offset = 0;
}

void Console::scroll_down() {
	int new_offset = 0;
	for (int i = 0; i < line_size; i++) {
		new_offset = scroll_offset + i;
		if (text_buffer[new_offset] == '\n') {
			new_offset++;
			break;
		}
	}
	if (new_offset > CON_BUFFER_SIZE) return;
	scroll_offset = new_offset;
}