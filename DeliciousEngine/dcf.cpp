#include "dcf.h"

#ifndef NULL
#define NULL 0
#endif

namespace dcf {

	uint32 str_len(cstring str) {
		uint32 len = 0;
		while (*str++) len++;
		return len;
	}

	cstring str_prev_glyph(cstring str, cstring min) {
		while (str-- != min) {
			if (is_glyph(*str)) return str;
		}
		return NULL;
	}
	cstring str_prev_wspace(cstring str, cstring min) {
		while (str-- != min) {
			if (!is_glyph(*str)) return str;
		}
		return NULL;
	}
	cstring str_next_word(cstring str) {
		while (*str++ != NULL) {
			if (*str == ' ' && *(str + 1) != NULL && is_glyph(*str + 1)) return str + 1;
		}
		return NULL;
	}

	bool is_glyph(const char c) {
		return (c > 32 && c < 127);
	}
}