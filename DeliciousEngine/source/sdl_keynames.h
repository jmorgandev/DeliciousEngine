#ifndef DELICIOUS_SDL_KEYNAMES_H
#define DELICIOUS_SDL_KEYNAMES_H

#include <vector>
#include <string>
#include <SDL/SDL_keyboard.h>
#include <algorithm>
#include <cctype>

constexpr SDL_Keycode exposed_sdl_keys[] = {
	SDLK_0, SDLK_1,	SDLK_2,	SDLK_3,
	SDLK_4,	SDLK_5,	SDLK_6,	SDLK_7,
	SDLK_8,	SDLK_9,

	SDLK_F1, SDLK_F2, SDLK_F3,
	SDLK_F4, SDLK_F5, SDLK_F6,
	SDLK_F7, SDLK_F8, SDLK_F9,
	SDLK_F10, SDLK_F11, SDLK_F12,

	SDLK_KP_0, SDLK_KP_1, SDLK_KP_2,
	SDLK_KP_3, SDLK_KP_4, SDLK_KP_5,
	SDLK_KP_6, SDLK_KP_7, SDLK_KP_8,
	SDLK_KP_9,

	SDLK_LGUI, SDLK_RGUI,

	SDLK_BACKSPACE,
	SDLK_CAPSLOCK,
	SDLK_DELETE,
	SDLK_END,
	SDLK_ESCAPE,
	SDLK_HOME,
	SDLK_INSERT,
	SDLK_NUMLOCKCLEAR,
	SDLK_PAGEDOWN,
	SDLK_PAGEUP,
	SDLK_PAUSE,
	SDLK_PRINTSCREEN,
	SDLK_RETURN,
	SDLK_SCROLLLOCK,
	SDLK_SPACE,
	SDLK_TAB,

	SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN,

	SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e,
	SDLK_f, SDLK_g, SDLK_h, SDLK_i, SDLK_j,
	SDLK_k, SDLK_l, SDLK_m, SDLK_n, SDLK_o,
	SDLK_p, SDLK_q, SDLK_r, SDLK_s, SDLK_t,
	SDLK_u, SDLK_v, SDLK_w, SDLK_x, SDLK_y,
	SDLK_z,

	SDLK_COMMA,
	SDLK_EQUALS,
	SDLK_MINUS,
	SDLK_PERIOD,
	SDLK_SEMICOLON,
	SDLK_BACKSLASH,
	SDLK_SLASH,
	SDLK_BACKQUOTE,
	SDLK_QUOTE,
	SDLK_LEFTBRACKET,
	SDLK_RIGHTBRACKET,

	// Shift modified
	SDLK_AMPERSAND,
	SDLK_ASTERISK,
	SDLK_AT,
	SDLK_CARET,
	SDLK_COLON,
	SDLK_DOLLAR,
	SDLK_EXCLAIM,
	SDLK_GREATER,
	SDLK_HASH,
	SDLK_LEFTPAREN,
	SDLK_LESS,
	SDLK_PERCENT,
	SDLK_PLUS,
	SDLK_QUESTION,
	SDLK_QUOTEDBL,
	SDLK_RIGHTPAREN,
	SDLK_UNDERSCORE,
};

std::vector<std::string> get_default_keynames() {
	using std::begin;
	using std::end;
	using std::transform;
	using std::remove;

	std::vector<std::string> result;

	for (auto k : exposed_sdl_keys) {
		std::string keyname(SDL_GetKeyName(k));
		transform(begin(keyname), end(keyname), begin(keyname), std::tolower);
		keyname.erase(remove(begin(keyname), end(keyname), ' '));
		result.push_back(keyname);
	}

	return result;
}

#endif