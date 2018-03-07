#ifndef DELICIOUS_DSTR_H
#define DELICIOUS_DSTR_H
#pragma once

#include <vector>

namespace dstr {
	char* trim(char* str);
	char* trim_front(char* str);
	char* trim_back(char* str);
	void  cut_extra_spaces(char* str);
	std::vector<const char*> split(char* str, char seperator);
	void split(char* str, char seperator, std::vector<const char*>& out);
}

#endif
