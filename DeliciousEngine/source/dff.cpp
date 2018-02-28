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
			result += line + "\n";
		}
		return result;
	}

	std::string get_glsl_region(std::string src, std::string label) {
		std::string search_term = "#scope " + label;
		size_t pos = src.find(search_term);
		if (pos == std::string::npos) return "";
		size_t region_start = pos + (search_term.size());
		size_t region_end = src.find("#scope ", region_start);
		return src.substr(region_start, region_end - region_start);
	}

	std::string get_glsl_header(std::string src) {
		size_t pos = src.find("#scope ");
		if (pos == std::string::npos) return "";
		return src.substr(0, pos);
	}
}