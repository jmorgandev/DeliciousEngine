#include "dff.h"

#include <fstream>
#include <string>

namespace dff {

	std::string path_extension(std::string filepath) {
		return filepath.substr(filepath.find_last_of('.'));
	}

	std::string path_filename(std::string filepath, bool keep_ext) {
		int from = filepath.find_last_of('/') + 1;
		if (keep_ext) return filepath.substr(from);
		int to = filepath.find_last_of('.');
		return filepath.substr(from, to - from);
	}

	std::string file_str(std::string filepath) {
		return file_str(filepath.c_str());
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