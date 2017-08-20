#ifndef DELICIOUS_DFF_H
#define DELICIOUS_DFF_H

#include <string>
#include <vector>
#include "dtypes.h"

namespace dff {
	std::string path_extension(std::string filepath);
	
	
	std::string path_filename(std::string filepath, bool keep_ext);

	std::string file_str(std::string filepath);
	std::string file_str(cstring filepath);

	std::string get_glsl_region(std::string src, std::string key, std::string label);
}

#endif