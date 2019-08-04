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

using std::make_pair;

static std::unordered_map<std::string, SDL_Keycode> keynames = {
	// Number keys
	make_pair("num0", SDLK_0),
	make_pair("num1", SDLK_1),
	make_pair("num2", SDLK_2),
	make_pair("num3", SDLK_3),
	make_pair("num4", SDLK_4),
	make_pair("num5", SDLK_5),
	make_pair("num6", SDLK_6),
	make_pair("num7", SDLK_7),
	make_pair("num8", SDLK_8),
	make_pair("num9", SDLK_9),

	//Numpad keys
	make_pair("keypad0", SDLK_KP_0),
	make_pair("keypad1", SDLK_KP_1),
	make_pair("keypad2", SDLK_KP_2),
	make_pair("keypad3", SDLK_KP_3),
	make_pair("keypad4", SDLK_KP_4),
	make_pair("keypad5", SDLK_KP_5),
	make_pair("keypad6", SDLK_KP_6),
	make_pair("keypad7", SDLK_KP_7),
	make_pair("keypad8", SDLK_KP_8),
	make_pair("keypad9", SDLK_KP_9),
	make_pair("enter", SDLK_KP_ENTER),
	make_pair("keypadPeriod", SDLK_KP_PERIOD),
	make_pair("keypadDivide", SDLK_KP_DIVIDE),
	make_pair("keypadMultiply", SDLK_KP_MULTIPLY),
	make_pair("keypadMinus", SDLK_KP_MINUS),
	make_pair("keypadPlus", SDLK_KP_PLUS),
	make_pair("keypadEquals", SDLK_KP_EQUALS),

	//Alpha keys
	make_pair("a", SDLK_a),
	make_pair("b", SDLK_b),
	make_pair("c", SDLK_c),
	make_pair("d", SDLK_d),
	make_pair("e", SDLK_e),
	make_pair("f", SDLK_f),
	make_pair("g", SDLK_g),
	make_pair("h", SDLK_h),
	make_pair("i", SDLK_i),
	make_pair("j", SDLK_j),
	make_pair("k", SDLK_k),
	make_pair("l", SDLK_l),
	make_pair("m", SDLK_m),
	make_pair("n", SDLK_n),
	make_pair("o", SDLK_o),
	make_pair("p", SDLK_p),
	make_pair("q", SDLK_q),
	make_pair("r", SDLK_r),
	make_pair("s", SDLK_s),
	make_pair("t", SDLK_t),
	make_pair("u", SDLK_u),
	make_pair("v", SDLK_v),
	make_pair("w", SDLK_w),
	make_pair("x", SDLK_x),
	make_pair("y", SDLK_y),
	make_pair("z", SDLK_z),

	//Function keys
	make_pair("f1", SDLK_F1),
	make_pair("f2", SDLK_F2),
	make_pair("f3", SDLK_F3),
	make_pair("f4", SDLK_F4),
	make_pair("f5", SDLK_F5),
	make_pair("f6", SDLK_F6),
	make_pair("f7", SDLK_F7),
	make_pair("f8", SDLK_F8),
	make_pair("f9", SDLK_F9),
	make_pair("f10", SDLK_F10),
	make_pair("f11", SDLK_F11),
	make_pair("f12", SDLK_F12),

	//Special keys
	make_pair("backspace", SDLK_BACKSPACE),
	make_pair("delete", SDLK_DELETE),
	make_pair("tab", SDLK_TAB),
	make_pair("return", SDLK_RETURN),
	make_pair("pause", SDLK_PAUSE),
	make_pair("escape", SDLK_ESCAPE),
	make_pair("space", SDLK_SPACE),
	make_pair("period", SDLK_PERIOD),
	make_pair("slash", SDLK_SLASH),
	make_pair("asterisk", SDLK_ASTERISK),
	make_pair("minus", SDLK_MINUS),
	make_pair("plus", SDLK_PLUS),
	make_pair("equals", SDLK_EQUALS),
	make_pair("up", SDLK_UP),
	make_pair("down", SDLK_DOWN),
	make_pair("left", SDLK_LEFT),
	make_pair("right", SDLK_RIGHT),
	make_pair("insert", SDLK_INSERT),
	make_pair("home", SDLK_HOME),
	make_pair("end", SDLK_END),
	make_pair("pageup", SDLK_PAGEUP),
	make_pair("pagedown", SDLK_PAGEDOWN),
	make_pair("quote", SDLK_QUOTE),
	make_pair("comma", SDLK_COMMA),
	make_pair("colon", SDLK_COLON),
	make_pair("semicolon", SDLK_SEMICOLON),
	make_pair("less", SDLK_LESS),
	make_pair("greater", SDLK_GREATER),
	make_pair("question", SDLK_QUESTION),
	make_pair("at", SDLK_AT),
	make_pair("leftParen", SDLK_LEFTPAREN),
	make_pair("rightParent", SDLK_RIGHTPAREN),
	make_pair("leftBracket", SDLK_LEFTBRACKET),
	make_pair("rightBracket", SDLK_RIGHTBRACKET),
	make_pair("caret", SDLK_CARET),
	make_pair("underscore", SDLK_UNDERSCORE),
	make_pair("backquote", SDLK_BACKQUOTE),
	make_pair("numlock", SDLK_NUMLOCKCLEAR),
	make_pair("capslock", SDLK_CAPSLOCK),
	make_pair("scrlock", SDLK_SCROLLLOCK),
	make_pair("leftshift", SDLK_LSHIFT),
	make_pair("rightshift", SDLK_RSHIFT),
	make_pair("leftctrl", SDLK_LCTRL),
	make_pair("rightctrl", SDLK_RCTRL),
	make_pair("leftalt", SDLK_LALT),
	make_pair("rightalt", SDLK_RALT),
	make_pair("leftgui", SDLK_LGUI),
	make_pair("rightgui", SDLK_RGUI),
	make_pair("printscr", SDLK_PRINTSCREEN)
};

bool Input::load() {
	key_records.reserve(10);
	setup_gui_bindings();
	SDL_StartTextInput();

	bind(SDLK_BACKQUOTE, "toggleconsole");

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

void Input::bind(SDL_Keycode keycode, cstring command) {
	for (auto bind : key_binds) {
		if (bind.keycode == keycode) {
			strcpy(bind.command, command);
			return;
		}
	}
	key_bind new_binding = { keycode, "" };
	strcpy(new_binding.command, command);
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
			engine.get<Console>().set_variable("eng_running", false);
			break;
		case SDL_KEYDOWN: case SDL_KEYUP:
			io.KeysDown[event.key.keysym.scancode] = (event.type == SDL_KEYDOWN);
			io.KeyShift = event.key.keysym.mod & KMOD_SHIFT;
			io.KeyCtrl  = event.key.keysym.mod & KMOD_CTRL;
			io.KeyAlt   = event.key.keysym.mod & KMOD_ALT;
			io.KeySuper = event.key.keysym.mod & KMOD_GUI;
			
			if (event.type == SDL_KEYDOWN && !io.WantCaptureKeyboard) {
				if (key_record* record = find_record(event.key.keysym.sym))
					record->state = KEY_HOLD;
				else {
					if (key_bind* bind = find_bind(event.key.keysym.sym))
						engine.get<Console>().execute_keybind(bind);
					key_records.push_back({ event.key.keysym.sym, KEY_PRESSED });
				}
			}
			if (event.type == SDL_KEYUP) {
				if (key_record* record = find_record(event.key.keysym.sym))
					record->state = KEY_RELEASED;
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