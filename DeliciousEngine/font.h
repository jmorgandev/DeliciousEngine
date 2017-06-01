#ifndef DELICIOUS_FONT_H
#define DELICIOUS_FONT_H

class Texture;

class Font {
public:
	Font(Texture* texture_in);
private:
	Texture* texture_ref;
};

#endif