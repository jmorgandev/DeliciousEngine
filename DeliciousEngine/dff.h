#ifndef DELICIOUS_DFF_H
#define DELICIOUS_DFF_H

#include <string>
#include "dtypes.h"

namespace dff {
	std::string path_extension(std::string filepath);
	cstring		path_extension(cstring filepath);
}

#endif