#ifndef DELICIOUS_FONT_H
#define DELICIOUS_FONT_H

class Texture;
class Shader;

class Font {
public:
	Font(Texture* texture_in, Shader* shader_in);
private:
	Texture* texture;
	Shader* shader;
};

#endif