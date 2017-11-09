#ifndef DELICIOUS_CONSOLE_H
#define DELICIOUS_CONSOLE_H

#include <vector>
#include <SDL/SDL_Events.h>
#include "console_types.h"
#include "font_renderer.h"
#include "box_renderer.h"
#include "system_ref.h"
#include "input_types.h"

#define CON_BUFFER_SIZE 2048
//#define CON_BUFFER_SIZE 16384
#define CON_INPUT_SIZE 128
#define CON_INPUT_LENGTH CON_INPUT_SIZE - 1
#define CON_INPUT_SCROLL_MULTIPLE	4

#define CON_HISTORY_SIZE 10

class Console {
public:
	bool init(System_Ref sys);
	void clean_exit();

	void load_config();
	void draw();

	void register_variable(cstring name, system_var* ref, cvar_type type, uint16 access_flags);
	void register_command(const console_cmd& cmd);

	system_var* read_variable(cstring name);
	void write_variable(cstring name, int value);
	void write_variable(cstring name, float value);
	void write_variable(cstring name, bool value);
	void write_variable(cstring name, system_var value, cvar_type type);

	void execute_keybind(key_bind* kb);

	void key_input(SDL_KeyboardEvent ev);
	void text_input(SDL_TextInputEvent ev);

	//Renderer properties
	void set_font(Font* fnt);
	void set_gui_properties(GLuint vao, Shader* shader);

	void clear();

	bool is_open();
	void display(bool d);
	void display_toggle();
	void display_reformat();

	//Operator overloads
	Console& operator<<(const bool& rhs);
	Console& operator<<(const char& rhs);
	Console& operator<<(const int& rhs);
	Console& operator<<(const float& rhs);
	Console& operator<<(const double& rhs);
	Console& operator<<(cstring rhs);
	Console& operator<<(const std::string& rhs);
private:
	System_Ref system;

	//GUI Renderers
	BoxRenderer	box_renderer;
	FontRenderer text_renderer;

	//Main text buffer variables
	char	text_buffer[CON_BUFFER_SIZE];
	uint16  buffer_extent;
	uint16	write_index;
	uint16	read_index;
	uint16	scroll_offset;

	//Input buffer variables
	char		input_buffer[CON_INPUT_SIZE];
	uint16		input_index;
	uint16		input_scroll;
	bool		input_insert;
	SDL_Keycode toggle_key;
	SDL_Keycode last_input;

	//History & Auto-complete variables
	///uint16	history_buffer[CON_HISTORY_SIZE][CON_INPUT_LENGTH];

	//Rendering variables
	int line_size;
	int visible_lines;
	int total_lines;
	uint8 border_x;
	uint8 border_y;

	bool display_console;

	//Console variable and command lists
	std::vector<console_var> variables;
	std::vector<console_cmd> commands;
	
	//Printing Functions
	void write_str(cstring str, bool new_line = false);
	void write_str(cstring str, uint32 size, bool new_line = false);
	void write_char(uchar c);
	void buffer_alloc();
	void terminate_current_line();

	console_var* find_variable(cstring name);
	console_cmd* find_command(cstring name);

	void set_variable(cstring name, cstring value);
	void set_variable(console_var* cvar, cstring value);

	void execute_string(cstring cmd_str);

	//Input Functions
	void write_to_input(cstring str);
	void execute_input(bool user_input);
	void clear_input();

	//User scroll functions
	bool scroll_up();
	bool scroll_down();
	void scroll_top();
	void scroll_bottom();
	bool scroll_left();
	bool scroll_right();
};

#endif