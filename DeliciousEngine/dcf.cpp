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
	cstring str_find(cstring str, char c) {
		while (*str++ != NULL) {
			if (*str == c) return str;
		}
		return NULL;
	}
	cstring str_find_last(cstring str, char c) {
		cstring sp = NULL;
		while (*str++ != NULL) {
			if (*str == c) sp = str;
		}
		return sp;
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
	cstring str_prev_instance(cstring str, cstring min, char c) {
		while (str-- != min) {
			if (*str == c) return str;
		}
		return NULL;
	}
	cstring str_next_word(cstring str) {
		while (*str++ != NULL) {
			if (*str == ' ' && is_glyph(*str + 1)) return str + 1;
		}
		return NULL;
	}

	bool is_glyph(const char c) {
		return (c > 32 && c < 127);
	}
	bool str_cmp(cstring lhs, cstring rhs) {
		while (*lhs && *rhs) {
			if (*(lhs++) != *(rhs++)) return false;
		}
		return true;
	}
	bool str_cmp_exact(cstring lhs, cstring rhs) {
		while (*lhs && *rhs) {
			if (*(lhs++) != *(rhs++)) return false;
		}
		return (*lhs == *rhs);
	}
	bool printable(const char c) {
		if (c >= ' ' && c <= '~') {
			return true;
		}
		else {
			return false;
		}
	}
}