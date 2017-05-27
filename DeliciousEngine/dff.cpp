#include "dff.h"

#include "dcf.h"

namespace dff {

	std::string path_extension(std::string filepath) {
		return filepath.substr(filepath.find_last_of('.'));
	}
	cstring path_extension(cstring filepath) {
		return dcf::str_find_last(filepath, '.');
	}
}