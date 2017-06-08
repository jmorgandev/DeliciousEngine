#ifndef DELICIOUS_RESOURCES_H
#define DELICIOUS_RESOURCES_H

#include "dtypes.h"
<<<<<<< HEAD
#include "shader.h"
#include "texture.h"
#include "font.h"
=======
#include <vector>
#include <string>
>>>>>>> parent of 668f43b... Use opengl context rather than SDL window

class Engine;
class Console;
class Texture;
class Shader;

class Resources {
public:
	bool init(Engine* engine_in);

<<<<<<< HEAD
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
=======
	void load_texture(std::string filepath);
	void load_shader(std::string filepath);
private:
	Console* console_ref;

	std::vector<Texture> texture_catalog;
	std::vector<Shader> shader_catalog;
>>>>>>> parent of 668f43b... Use opengl context rather than SDL window
};

#endif