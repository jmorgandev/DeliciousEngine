#include "dff.h"

#include <fstream>
#include "dcf.h"

namespace dff {

	std::string path_extension(std::string filepath) {
		return filepath.substr(filepath.find_last_of('.'));
	}
	cstring path_extension(cstring filepath) {
		return dcf::str_find_last(filepath, '.');
	}

	std::string file_str(std::string filepath) {
		file_str(filepath.c_str());
	}
	std::string file_str(cstring filepath) {
		std::ifstream file_stream(filepath);
		if (!file_stream.is_open()) return "";

		std::string result;
		std::string line;
		while (std::getline(file_stream, line)) {
			result += line;
		}
		return result;
	}
}