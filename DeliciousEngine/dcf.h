#ifndef DELICIOUS_DCF_H
#define DELICIOUS_DCF_H

#include "dtypes.h"

namespace dcf {
	cstring str_find(cstring str, char c);
	cstring str_find_last(cstring str, char c);

	cstring str_prev_glyph(cstring str, cstring min);
	cstring str_prev_wspace(cstring str, cstring min);
	cstring str_prev_instance(cstring str, cstring min, char c);
	cstring str_prev_instance(cstring str, cstring min, cstring chrs);
	cstring str_next_word(cstring str);

	uint32 str_len(cstring str);
	uint32 str_count(cstring str, char c);
	uint32 str_word_len(cstring str);

	bool is_glyph(const char c);
	bool is_wspace(const char c);
	bool str_cmp(cstring lhs, cstring rhs);
	bool str_cmp_exact(cstring lhs, cstring rhs);
	bool str_contains(cstring str, char c);
	bool printable(const char c);

	void str_cpy(cstring src, char* dst);
}

#endif