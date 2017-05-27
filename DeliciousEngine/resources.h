#ifndef DELICIOUS_RESOURCES_H
#define DELICIOUS_RESOURCES_H

#include "dtypes.h"

class Engine;
class Resources {
public:
	bool init(Engine* engine_in);

	void load(cstring filepath);

private:
	void load_texture(cstring filepath);
};

#endif