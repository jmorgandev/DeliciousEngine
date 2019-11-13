#include "console.h"

#include <imgui.h>

#include "engine.h"
#include "screen.h"
#include "dstr.h"
#include "cmds.h"
#include "input.h"

bool Console::load() {
	display_console = false;

	return true;
}

bool Console::start() {
	//engine.get<Input>().bind(SDLK_BACKQUOTE, [this]() { display_toggle(); });
	
	return true;
}

void Console::update() {
	if (engine.get<Input>().key_pressed(SDLK_BACKQUOTE)) {
		display_toggle();
	}
}

bool Console::free() {
	//@Todo: Write CVars to config file.
	variables.clear();
	return true;
}

void Console::update_and_draw() {
	constexpr uint WIN_FLAGS = (ImGuiWindowFlags_NoCollapse | 
								ImGuiWindowFlags_NoSavedSettings);

	auto& screen = engine.get<Screen>();
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
			ImGui::SetScrollHereY();
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

// Copies a string directly to the input buffer
void Console::write_to_input(cstring str) {
	assert(strlen(str) <= CON_INPUT_LENGTH);
	strcpy(input_buffer, str);
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
	//execute_string(kb->command);
}

void Console::execute_string(cstring cmd_str) {
	
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

void Console::display(bool d) {
	display_console = d;
}

void Console::display_toggle() {
	display_console = !display_console;
}