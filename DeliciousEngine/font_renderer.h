#ifndef DELICIOUS_FONT_RENDERER_H
#define DELICIOUS_FONT_RENDERER_H

#include "dtypes.h"
#include <string>

class Font;

class FontRenderer {
public:
	FontRenderer();

	void begin();
	void draw_string(const char* str, float x, float y);
	void draw_char(char c, float x, float y);
	void end();

	void set_font(Font* font_in);
	Font* get_font();
private:
	Font* font;
};

#endif