#ifndef DELICIOUS_DCF_H
#define DELICIOUS_DCF_H

#include "dtypes.h"
#include <vector>

namespace dcf {
	cstring str_find(cstring str, char c);
	char*	str_find(char* str, char c);
	cstring str_find_last(cstring str, char c);

	cstring str_prev_glyph(cstring str, cstring min);
	cstring str_prev_wspace(cstring str, cstring min);
	cstring str_prev_instance(cstring str, cstring min, char c);
	cstring str_prev_instance(cstring str, cstring min, cstring chrs);
	cstring str_next_word(cstring str);
	char*	str_next_word(char* str);
	cstring str_next_glyph(cstring str);
	char*	str_next_glyph(char* str);
	cstring str_first_glyph(cstring str);
	char*   str_first_glyph(char* str);
	cstring str_end(cstring str);
	char*   str_end(char* str);

	uint32 str_len(cstring str);
	uint32 str_count(cstring str, char c);
	uint32 str_word_len(cstring str);

	bool is_glyph(const char c);
	bool is_wspace(const char c);
	bool str_cmp(cstring lhs, cstring rhs);
	bool str_cmp_exact(cstring lhs, cstring rhs);
	bool str_contains(cstring str, char c);
	bool is_printable(const char c);

	void str_cpy(cstring src, char* dst);
	void str_fill(char* src, char c, int amount);
	void str_trim_spaces(char* str);
	void str_shift_left(char* str, uint index);
	void str_shift_right(char* str, uint index);
	void str_split(char* str, char seperator);

	char** str_split(char* start, char* end, char seperator);

	std::vector<cstring> str_split_vector(char* str, char seperator);
	void str_split_vector(char* src, std::vector<cstring>& dest, char seperator);
}

#endif