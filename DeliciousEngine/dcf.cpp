#include "dcf.h"

#ifndef NULL
#define NULL 0
#endif

namespace dcf {
	cstring str_find(cstring str, char c) {
		while (*str != NULL) {
			if (*str++ == c) return str;
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
	cstring str_prev_instance(cstring str, cstring min, cstring chrs) {
		while (str-- != min) {
			if (dcf::str_contains(chrs, *str)) {
				return str;
			}
		}
		return NULL;
	}
	cstring str_next_word(cstring str) {
		while (*str++ != NULL) {
			if (*str == ' ' && is_glyph(*str + 1)) return str + 1;
		}
		return NULL;
	}
	uint32 str_len(cstring str) {
		uint32 len = 0;
		while (*str++) len++;
		return len;
	}
	uint32 str_count(cstring str, char c) {
		int result = 0;
		while (*str != NULL) {
			if (*str++ == c) result++;
		}
		return result;
	}
	uint32 str_word_len(cstring str) {
		uint32 len = 0;
		for (; *str != NULL && *str != ' '; str++) len++;
		return len;
	}
	bool is_glyph(const char c) {
		return (c > 32 && c < 127);
	}
	bool is_wspace(const char c) {
		return (c == ' ' || c == '\n' || c == '\t');
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
	bool str_contains(cstring str, char c) {
		while (*str != NULL) {
			if (*str++ == c) return true;
		}
		return false;
	}
	bool printable(const char c) {
		if (c >= ' ' && c <= '~') {
			return true;
		}
		else {
			return false;
		}
	}
	void str_cpy(cstring src, char* dst) {
		while (*src) {
			*dst++ = *src++;
		}
	}
}