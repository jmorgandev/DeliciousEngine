#include "input.h"

#include "console.h"
#include "dcf.h"
#include <SDL_events.h>
#include <algorithm>
#include <imgui.h>
#include <SDL_clipboard.h>

static void gui_set_clipboard_bind(void*, const char* text) {
	SDL_SetClipboardText(text);
}
static const char* gui_get_clipboard_bind(void*) {
	return SDL_GetClipboardText();
}

Input::Input() {
	
}

bool Input::init() {
	key_records.reserve(10);

	setup_gui_bindings();

	SDL_StartTextInput();

	return true;
}

void Input::setup_gui_bindings() {
	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab]		   = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow]  = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow]    = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow]  = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp]     = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown]   = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home]       = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End]        = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert]     = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete]     = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace]  = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space]      = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter]      = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape]     = SDL_SCANCODE_ESCAPE;
	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

	io.SetClipboardTextFn = gui_set_clipboard_bind;
	io.GetClipboardTextFn = gui_get_clipboard_bind;
	io.ClipboardUserData = NULL;

	io.IniFilename = NULL;
}

void Input::clean_exit() {
	SDL_StopTextInput();

	//@TODO: Dump keybinds to config files.
}

void Input::bind(SDL_Keycode keycode, cstring command) {
	for (auto bind : key_binds) {
		if (bind.keycode == keycode) {
			dcf::str_cpy(command, bind.command);
			return;
		}
	}
	key_bind new_binding = { keycode, "" };
	dcf::str_cpy(command, new_binding.command);
	key_binds.push_back(new_binding);
}

void Input::unbind(SDL_Keycode keycode) {
	for (auto it = key_binds.begin(); it != key_binds.end(); it++) {
		if (it->keycode == keycode) {
			key_binds.erase(it);
			break;
		}
	}
}

void Input::process_events() {
	update_records();

	SDL_Event event;
	ImGuiIO& io = ImGui::GetIO();
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			console.write_variable("eng_running", false);
			break;
		case SDL_KEYDOWN: case SDL_KEYUP:
			if (io.WantCaptureKeyboard) {
				io.KeysDown[event.key.keysym.scancode] = (event.type == SDL_KEYDOWN);
				io.KeyShift = event.key.keysym.mod & KMOD_SHIFT;
				io.KeyCtrl  = event.key.keysym.mod & KMOD_CTRL;
				io.KeyAlt   = event.key.keysym.mod & KMOD_ALT;
				io.KeySuper = event.key.keysym.mod & KMOD_GUI;
			}
			else {
				if (key_record* record = find_record(event.key.keysym.sym)) {
					record->state = (event.type == SDL_KEYDOWN) ? KEY_HOLD : KEY_RELEASED;
				}
				else if (event.type == SDL_KEYDOWN) {
					if (key_bind* bind = find_bind(event.key.keysym.sym)) {
						console.execute_keybind(bind);
					}
					key_records.push_back({ event.key.keysym.sym, KEY_PRESSED });
				}
			}
			break;
		case SDL_TEXTINPUT:
			io.AddInputCharactersUTF8(event.text.text);
			break;
		case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
			if (io.WantCaptureMouse) {
				io.MouseDown[event.button.button - 1] = (event.type == SDL_MOUSEBUTTONDOWN);
			}
			else mouse_buttons[event.button.button - 1] = (event.type == SDL_MOUSEBUTTONDOWN);
			break;
		case SDL_MOUSEWHEEL:
			if (io.WantCaptureMouse) {
				if (event.wheel.x > 0) io.MouseWheelH += 1;
				else if (event.wheel.x < 0) io.MouseWheelH -= 1;
				if (event.wheel.y > 0) io.MouseWheel += 1;
				else if (event.wheel.y < 0) io.MouseWheel -= 1;
			}
			break;
		case SDL_MOUSEMOTION:
			io.MousePos.x = (float)event.motion.x;
			io.MousePos.y = (float)event.motion.y;
			mouse_motion.x = (float)event.motion.xrel;
			mouse_motion.y = (float)event.motion.yrel;
			break;
		}
	}
}

key_bind* Input::find_bind(SDL_Keycode key) {
	for (auto it = key_binds.begin(); it != key_binds.end(); it++) {
		if (it->keycode == key) {
			return &(*it);
		}
	}
	return nullptr;
}

key_record* Input::find_record(SDL_Keycode key) {
	auto it = key_records.begin();
	for (auto it = key_records.begin(); it != key_records.end(); it++) {
		if (it->keycode == key) {
			return &(*it);
		}
	}
	return nullptr;
}

void Input::update_records() {
	for (auto it = key_records.begin(); it != key_records.end();) {
		if (it->state == KEY_RELEASED) {
			it = key_records.erase(it);
		}
		else it++;
	}
}

bool Input::get_key(SDL_Keycode keycode) {
	return (find_record(keycode) != nullptr);
}