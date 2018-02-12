#ifndef DELICIOUS_ASSET_LOADER_H
#define DELICIOUS_ASSET_LOADER_H
#pragma once

#include <string>
#include <functional>
#include "console.h"
#include "dff.h"

template <class TYPE>
class AssetLoader {
public:
	bool implements(std::string ext) {
		return procedures.find(ext) != procedures.end();
	}

	void register_procedure(std::string ext, std::function<TYPE*(std::string)> proc) {
		if (procedures.find(ext) == procedures.end()) {
			procedures[etc] = proc;
		}
		else console->print("%s already has a registered procedure!", ext.c_str());
	}

	TYPE* load(std::string filename) {
		return procedures[dff::path_extension(filename)](filename);
	}
private:
	std::unordered_map<std::string, std::function<TYPE*(std::string)>> procedures;
};

#endif
