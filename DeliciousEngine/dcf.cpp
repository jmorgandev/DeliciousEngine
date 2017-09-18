#include "dcf.h"

#ifndef NULL
#define NULL 0
#endif

namespace dcf {
	cstring str_find(cstring str, char c) {
		while (*str != NULL) {
			if (*str == c) return str;
			str++;
		}
		return NULL;
	}
	char* str_find(char* str, char c) {
		while (*str != NULL) {
			if (*str == c) return str;
			str++;
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
	char* str_next_word(char* str) {
		while (*str++ != NULL) {
			if (*str == ' ' && is_glyph(*str + 1)) {
				return str + 1;
			}
		}
		return NULL;
	}
	cstring str_next_glyph(cstring str) {
		while (*str++ != NULL) {
			if (is_glyph(*str)) return str;
		}
		return NULL;
	}
	char* str_next_glyph(char* str) {
		while (*str++ != NULL) {
			if (is_glyph(*str)) return str;
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
		*dst = '\0';
	}
	void str_fill(char* src, char c, int amount) {
		for (int i = 0; i < amount; i++) {
			src[i] = c;
		}
	}
	void str_trim_spaces(char* str) {
		char* sp = str;
		if (is_glyph(*str) == false) {
			str = str_next_glyph(str);
		}
		while (*str != NULL) {
			if (is_glyph(*str)) {
				*sp++ = *str++;
			}
			else {
				if (!(str = str_next_glyph(str))) *sp++ = NULL;
				else *sp++ = ' ';
			}
		}
		while (sp != str) *sp++ = NULL;
	}
	void str_shift_left(char* str, uint index) {
		uint str_size = str_len(str);
		for (uint i = index; i < str_size; i++) {
			str[i] = str[i + 1];
		}
	}
	void str_split(char* str, char seperator) {
		while (*str != NULL) {
			if (*str == seperator) {
				*str == '\0';
			}
			str++;
		}
	}
}