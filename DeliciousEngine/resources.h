#ifndef DELICIOUS_RESOURCES_H
#define DELICIOUS_RESOURCES_H

#include <string>
#include <unordered_map>
#include "dtypes.h"
#include "shader.h"
#include "texture.h"
#include "font.h"

class Engine;
class Console;

class Resources {
public:
	bool init(Engine* engine_in);

	Texture* load_texture(std::string filepath);
	Shader* load_shader(std::string filepath);

	Font* make_font(std::string font_name, Texture* texture_in, Shader* shader_in);
	
private:
	Console* console_ref;

	typedef std::unordered_map<std::string, Texture>::value_type texture_keypair;
	std::unordered_map<std::string, Texture> texture_catalog;

	typedef std::unordered_map<std::string, Shader>::value_type shader_keypair;
	std::unordered_map<std::string, Shader> shader_catalog;

	typedef std::unordered_map<std::string, Font>::value_type font_keypair;
	std::unordered_map<std::string, Font> font_catalog;
};

#endif