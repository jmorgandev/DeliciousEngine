#include "console.h"

#include <imgui.h>

#include "engine.h"
#include "screen.h"
#include "dstr.h"
#include "cmds.h"

//@Todo: Completely move away from global console command function definitions towards using member
//       function definitions instead...

ConsoleCommand(toggleconsole) {
	//console.display_toggle();
}

ConsoleCommand(clear) {
	//@Todo: just clear the vector of strings...
}
ConsoleCommand(quit) {
	//@Todo: Put this command in main.cpp to avoid write_variable overhead
	//console.set_variable("eng_running", false);
}

bool Console::load() {
	display_console = false;

	//register_command("toggleconsole", cmd_toggleconsole);
	//register_command("clear", cmd_clear);
	//register_command("quit",  cmd_quit);

	register_command("toggleconsole", cmd_toggleconsole);

	return true;
}

bool Console::free() {
	//@Todo: Write CVars to config file.
	variables.clear();
	commands.clear();
	return true;
}

static const uint WIN_FLAGS = (ImGuiWindowFlags_NoCollapse|
							   ImGuiWindowFlags_NoSavedSettings);
void Console::update_and_draw() {
	auto screen = engine.get<Screen>();
	if (display_console) {
		ImGui::SetNextWindowPos(screen.imgui_center(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(screen.imgui_size(0.75f), ImGuiCond_Once);

		ImGui::Begin("Console##console-window", &display_console, WIN_FLAGS);
		const float h = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("##console-report", ImVec2(0, -h), false);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		ImGuiListClipper clipper((int)report_text.size());
		while (clipper.Step()) {
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
				ImGui::TextWrapped(report_text[i].c_str());
			}
		}
		if (scroll_to_bottom) {
			ImGui::SetScrollHere();
			scroll_to_bottom = false;
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();
		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("##console-input", input_buffer, CON_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (strlen(input_buffer) > 0) {
				execute_input(true);
				scroll_to_bottom = true;
			}
			ImGui::SetKeyboardFocusHere(-1);
		}
		ImGui::PopItemWidth();
		ImGui::End();
	}
}

#define FORMAT_STR_BUFFER 1024
void Console::printf(cstring format, ...) {
	va_list args;
	char buffer[FORMAT_STR_BUFFER];

	va_start(args, format);
	uint length = vsprintf_s(buffer, format, args);
	va_end(args);

	report_text.push_back(buffer);
}

//Copies a string directly to the input buffer
void Console::write_to_input(cstring str) {
	assert(strlen(str) <= CON_INPUT_LENGTH);
	strcpy(input_buffer, str);
}

/*
Attempts to register a variable to the variable list. If one already
exists with the same name, print an error to the console.
*/
void Console::register_variable(cstring name, SystemVar* ptr, CvarType t, uint16 access_flags) {
	if (!find_variable(name)) {
		assert(strlen(name) <= CON_MAX_NAME);
		ConsoleVar new_cvar = { "", ptr, t, access_flags };
		strcpy(new_cvar.name, name);
		variables.push_back(new_cvar);
	}
	else printf("register_variable: \"%s\" already exists!", name);
}

/*
Attempts to register a command to the command list. If one already exists
with the same name, print an error to the console.
*/
void Console::register_command(cstring name, CmdFunc func) {
	if (!find_command(name)) {
		assert(strlen(name) <= CON_MAX_NAME);
		ConsoleCmd new_cmd = { "", func, CMD_CPP };
		strcpy(new_cmd.name, name);
		commands.push_back(new_cmd);
	}
	else printf("register_cmd: \"%s\" already exists.", name);
}

/*
Attempts to fetch the pointer of a registered variable. Only returns a
valid pointer if the names match exactly.
*/
ConsoleVar* Console::find_variable(cstring name) {
	using namespace std;
	for (auto it = begin(variables); it != end(variables); it++) {
		if (strcmp(name, it->name) == 0) return &(*it);
	}
	return nullptr;
}

/*
Attempts to fetch the pointer of a registered command. Only returns a
valid pointer if the names match exactly.
*/
ConsoleCmd* Console::find_command(cstring name) {
	using namespace std;
	for (auto it = begin(commands); it != end(commands); it++) {
		if (strcmp(name, it->name) == 0) return &(*it);
	}
	return nullptr;
}

SystemVar* Console::get_variable(cstring name) {
	if (ConsoleVar* cvar = find_variable(name)) return cvar->data;
	printf("get_variable: \"%s\" does not exist!", name);
	return nullptr;
}

void Console::write_variable(cstring name, SystemVar var, CvarType t) {
	if (ConsoleVar* cvar = find_variable(name)) {
		if (cvar->type == t) *cvar->data = var;
		else printf("write_variable: \"%s\" type mismatch!", name);
	} 
	else printf("write_variable: \"%s\" does not exist!", name);
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
	execute_string(kb->command);
}

void Console::execute_string(cstring cmd_str) {
	//@Todo: implement command chaining with ';'
	char buffer[CON_INPUT_SIZE];
	strcpy(buffer, cmd_str);
	char* label = dstr::trim(buffer);
	dstr::cut_extra_spaces(label);

	std::vector<cstring> argv;

	char* args = strchr(label, ' ');
	if (args != nullptr) {
		*(args++) = 0;
		dstr::split(args, ' ', argv);
	}

	if (ConsoleVar* cvar = find_variable(label)) {
		if (argv.empty()) {
			switch (cvar->type) {
			case CVAR_INT:
				printf("%s is %i", cvar->name, cvar->data->as_int);
				break;
			case CVAR_FLOAT:
				printf("%s is %f", cvar->name, cvar->data->as_float);
				break;
			case CVAR_BOOL:
				printf("%s is %i", cvar->name, cvar->data->as_bool);
				break;
			}
		}
		else if (argv.size() == 1) assign_variable(cvar, argv[0]);
		else printf("Set variable usage: <var> <value>");
	}
	else if (ConsoleCmd* cmd = find_command(label))
			cmd->callback(argv);
	else 
		printf("Unknown command/variable: \"%s\"", label);
}

/*
Wipes the input buffer with null characters and resets the input index.
*/
void Console::clear_input() {
	*input_buffer = 0;
}

bool Console::is_open() {
	return display_console;
}

void Console::load_config() {
	//@Todo: implement
}

void Console::assign_variable(cstring name, cstring value) {
	if (ConsoleVar *cvar = find_variable(name)) assign_variable(cvar, value);
	else printf("set_variable: \"%s\" does not exist!", name);
}

void Console::assign_variable(ConsoleVar* cvar, cstring value) {
	if (cvar->flags & CVAR_MUTABLE) {
		switch (cvar->type) {
		case CVAR_INT:
			cvar->data->as_int = std::atoi(value);
			break;
		case CVAR_FLOAT:
			cvar->data->as_float = (float)std::atof(value);
			break;
		case CVAR_BOOL:
			if (value[0] == 't' || value[0] == 'T' || std::atoi(value) > 0) cvar->data->as_bool = true;
			else cvar->data->as_bool = false;
			break;
		}
	}
	else printf("%s is read-only.", cvar->name);
}

void Console::display(bool d) {
	display_console = d;
}

void Console::display_toggle() {
	display_console = !display_console;
}