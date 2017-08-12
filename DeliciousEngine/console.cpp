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

void Console::write_str(cstring str) { write_str(str, dcf::str_len(str)); }
//
// Main writing function, handles the buffer and any line deletion / wrapping / overflows
//
void Console::write_str(cstring str, uint32 size) {
	buffer_alloc(size);		//Make sure there is enough space for the string to overwrite the buffer contents
	while (*str != NULL) {	//Keep looping until no more words left in string

		int line_remaining = line_size - (front_index % line_size);
		int str_remaining = dcf::str_len(str);

		if (str_remaining > line_remaining) {		//Check if wrapping needs to occur
			if (str[line_remaining - 1] == ' ') {	//Handle overflow caused by a space
				for (; line_remaining > 0; line_remaining--) write_char(*str++);
				str = dcf::str_next_word(str);
			}
			else {				//Handle overflow caused by glyph
				cstring wrap_point = dcf::str_prev_glyph(str + (line_remaining - 1), str) + 1;
				if(wrap_point == NULL) {	//Handle a string without spaces by printing without wrapping
					for (; line_remaining > 0; line_remaining--) write_char(*str++);
				}
				else {			//Handle wrapping by splitting string at earliest possible space		
					while (str++ != wrap_point) write_char(*str);
					write_char('\n'); str = dcf::str_next_word(str); //Instead of the space, put a new line.
																	 //Then advance to next word in string.
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
	front_index = front_index++ % CON_BUFFER_SIZE;
}

void Console::buffer_alloc(uint32 size) {
	uint16 space = math::delta(back_index, front_index);
	if (size >= space) {
		back_index = (back_index + (size - space)) % CON_BUFFER_SIZE;
		while (text_buffer[back_index] != '\n') {
			back_index = back_index + 1 % CON_BUFFER_SIZE;
		}
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

//void Console::write_to_input(cstring str) {
//	cstring sp = str;
//	while (*sp != NULL && input_index != CON_INPUT_SIZE) {
//		input_buffer[input_index++] = *sp++;
//	}
//}
//
//void Console::execute_input(bool user_input) {
//	if (user_input) {
//		write_str(input_buffer, dcf::str_len(input_buffer) + 1);
//		write_char('\n');
//	}
//	
//}

void Console::set_font(Font* fnt) {
	text_renderer.set_font(fnt);

	line_size = (engine->get_screen()->get_width() / fnt->cell_width) - (border_x * 2);
	visible_lines = (engine->get_screen()->get_height() / fnt->cell_height) - (border_y * 2);
}

void Console::render() {
	Screen* scr = engine->get_screen();
	Font* fnt = text_renderer.get_font();
	text_renderer.begin(scr->get_width(), scr->get_height());
	int render_index = (back_index % CON_BUFFER_SIZE);
	for (int y = 0; y < visible_lines; y++) {
		for (int x = 0; x < line_size; x++) {
			char current = text_buffer[render_index];
			if (text_buffer[render_index] == '\n') {
				render_index++;
				break;
			}
			if (text_buffer[render_index] == '\0') {
				return;
			}
			//@TODO - Rendering is two times smaller, why is that? Do some math or something.
			text_renderer.draw_char(text_buffer[render_index], (x + border_x) * fnt->cell_width, (y + border_y) * fnt->cell_height);
			render_index++;
		}
	}
	text_renderer.end();
}