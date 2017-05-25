#ifndef DCF_H
#define DCF_H

#include "dtypes.h"

namespace dcf {
	uint32	str_len(cstring str);

	cstring str_prev_glyph(cstring str, cstring min);
	cstring str_prev_wspace(cstring str, cstring min);
	cstring str_next_word(cstring str);

	bool is_glyph(const char c);
	bool str_cmp(cstring lhs, cstring rhs);
	bool str_cmp_exact(cstring lhs, cstring rhs);
}

#endif