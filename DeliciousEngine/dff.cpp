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

	std::vector<std::string> get_gls_regions(std::string src) {
		std::vector<std::string> result;
		size_t pos = src.find("//:");
		while (pos != std::string::npos) {
			size_t region_start = pos + 3;
			size_t region_end = src.find("\n", region_start);
			result.push_back(src.substr(region_start, region_end - region_start));
			pos = src.find("//:", region_end);
		}
		return result;
	}

	std::string get_gls_region(std::string src, std::string region) {
		size_t pos = src.find("//:" + region);
		if (pos == std::string::npos) return "";
		size_t region_start = pos + (region.size() + 3);
		size_t region_end = src.find("//:", region_start);
		return src.substr(region_start, region_end - region_start);
	}
}