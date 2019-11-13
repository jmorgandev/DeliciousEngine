#include "input.h"

#include "engine.h"

#include <algorithm>
#include <unordered_map>

#include <imgui.h>
#include <SDL_events.h>
#include <SDL_clipboard.h>

#include "console.h"

static void gui_set_clipboard_bind(void*, const char* text) {
	SDL_SetClipboardText(text);
}
static const char* gui_get_clipboard_bind(void*) {
	return SDL_GetClipboardText();
}

#include "sdl_keynames.h"

bool Input::load() {
	records.reserve(10);
	setup_gui_bindings();
	SDL_StartTextInput();

	auto keynames = get_default_keynames();

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

bool Input::free() {
	SDL_StopTextInput();
	//@Todo: Dump keybinds to config files.
	return true;
}

void Input::process_events() {
	flush();

	SDL_Event event;
	ImGuiIO& io = ImGui::GetIO();
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			engine.quit();
			break;
		case SDL_KEYDOWN: case SDL_KEYUP:
			auto keysym = event.key.keysym;
			io.KeysDown[keysym.scancode] = (event.type == SDL_KEYDOWN);
			io.KeyShift = keysym.mod & KMOD_SHIFT;
			io.KeyCtrl  = keysym.mod & KMOD_CTRL;
			io.KeyAlt   = keysym.mod & KMOD_ALT;
			io.KeySuper = keysym.mod & KMOD_GUI;

			if (event.type == SDL_KEYDOWN && !io.WantCaptureKeyboard) {
				if (!find_record(keysym.sym)) {
					records.push_back({keysym.sym, KEY_PRESSED});
				}
			}
			else if (event.type == SDL_KEYUP && !io.WantCaptureKeyboard) {
				if (KeyRecord* record = find_record(keysym.sym)) {
					record->state = KEY_RELEASED;
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

KeyRecord* Input::find_record(SDL_Keycode key) {
	for (auto& record : records) {
		if (record.keycode == key)
			return &record;
	}
	return nullptr;
}


static bool flush_predicate(const KeyRecord & r) { return r.state == KEY_RELEASED; }
static KeyRecord hold_transform(KeyRecord r) { if (r.state == KEY_PRESSED) { r.state = KEY_HELD; } return r; }
void Input::flush() {
	std::transform(begin(records), end(records), begin(records), hold_transform);
	auto it = remove_if(begin(records), end(records), flush_predicate);

	if (it != end(records)) {
		records.erase(it);
	}
}

bool Input::get_key(SDL_Keycode keycode) {
	return (find_record(keycode) != nullptr);
}

bool Input::check_key_state(SDL_Keycode keycode, KeyState state) {
	if (auto record = find_record(keycode)) {
		return record->state == state;
	}
	return false;
}

bool Input::key_pressed(SDL_Keycode keycode) {
	return check_key_state(keycode, KEY_PRESSED);
}

bool Input::key_held(SDL_Keycode keycode) {
	return check_key_state(keycode, KEY_HELD);
}

bool Input::key_released(SDL_Keycode keycode) {
	return check_key_state(keycode, KEY_RELEASED);
}