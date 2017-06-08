#ifndef DELICIOUS_FONT_RENDERER_H
#define DELICIOUS_FONT_RENDERER_H

#include "dtypes.h"
#include <string>

class Font;

class FontRenderer {
public:
	FontRenderer();

	void draw_char(char c, float x, float y);

	void set_font(Font* font_in);
	Font* get_font();
private:
	Font* font;
};

#endif