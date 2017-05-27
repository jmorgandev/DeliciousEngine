#ifndef DELICIOUS_RESOURCES_H
#define DELICIOUS_RESOURCES_H

#include "dtypes.h"

class Engine;
class Console;
class Texture;

class Resources {
public:
	bool init(Engine* engine_in);

	void load_texture(cstring filepath);
	void load_shader(cstring filepath);
private:
	Console* console_ref;
};

#endif