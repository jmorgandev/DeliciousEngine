#ifndef DELICIOUS_DFF_H
#define DELICIOUS_DFF_H

#include <string>
#include "dtypes.h"

namespace dff {
	std::string path_extension(std::string filepath);
	cstring		path_extension(cstring filepath);
	
	std::string path_filename(std::string filepath, bool keep_ext);

	std::string file_str(std::string filepath);
	std::string file_str(cstring filepath);
}

#endif