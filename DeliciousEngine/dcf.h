#ifndef DELICIOUS_DCF_H
#define DELICIOUS_DCF_H

#include "dtypes.h"

namespace dcf {
	uint32	str_len(cstring str);

	cstring str_find(cstring str, char c);
	cstring str_find_last(cstring str, char c);

	cstring str_prev_glyph(cstring str, cstring min);
	cstring str_prev_wspace(cstring str, cstring min);
	cstring str_next_word(cstring str);

	bool is_glyph(const char c);
	bool str_cmp(cstring lhs, cstring rhs);
	bool str_cmp_exact(cstring lhs, cstring rhs);

	bool printable(const char c);
}

#endif