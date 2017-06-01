#ifndef DELICIOUS_RESOURCES_H
#define DELICIOUS_RESOURCES_H

#include "dtypes.h"
#include <vector>
#include <string>

class Engine;
class Console;
class Texture;
class Shader;

class Resources {
public:
	bool init(Engine* engine_in);

	void load_texture(std::string filepath);
	void load_shader(std::string filepath);
private:
	Console* console_ref;

	std::vector<Texture> texture_catalog;
	std::vector<Shader> shader_catalog;
};

#endif