#include "dcf.h"

namespace dcf {

	uint32 str_len(cstring str) {
		uint32 len = 0;
		while (*str++) len++;
		return len;
	}

}