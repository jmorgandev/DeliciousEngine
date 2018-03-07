#include "dstr.h"

#include <string.h>

namespace dstr {

	char* trim(char* str) {
		str = trim_front(str);
		str = trim_back(str);
		return str;
	}
	char* trim_front(char* str) {
		while (*str == ' ') str++;
		return str;
	}
	char* trim_back(char* str) {
		char* end = str + strlen(str) - 1;
		while (end != str && *end == ' ') end--;
		*(end + 1) = 0;
		return str;	//Returning is unnecessary, but avoids confusion
	}
	void cut_extra_spaces(char* str) {
		char* sp = str;
		while (*str != 0) {
			if (*str == ' ' && *(str + 1) == ' ') str++;
			else *sp++ = *str++;
		}
		if (sp != str) *sp = 0;
	}
	std::vector<const char*> split(char* str, char seperator) {
		std::vector<const char*> result;
		split(str, seperator, result);
		return result;
	}
	void split(char* str, char seperator, std::vector<const char*>& out) {
		char* prev = str;
		while (*str != 0) {
			if (*str == seperator) {
				*str = 0;
				out.push_back(prev);
				prev = str + 1;
			}
			str++;
		}
		out.push_back(prev);
	}
}