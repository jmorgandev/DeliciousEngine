#ifndef DELICIOUS_FONT_RENDERER_H
#define DELICIOUS_FONT_RENDERER_H

#include "dtypes.h"
#include <string>

struct Font;
class FontRenderer {
public:
	FontRenderer();

	void begin(int screen_w, int screen_h);
	void draw_string(const char* str, float x, float y);
	void draw_char(char c, int pixel_x, int pixel_y);
	void end();

	void set_font(Font* font_in);
	Font* get_font();
private:
	Font* font;
	float render_width;
	float render_height;
	float texcell_width;
	float texcell_height;
	float pixel_width;
	float pixel_height;

	int uniform_translation;
	int uniform_scale;
	int uniform_tex_offset;
	int uniform_tex_scale;
};

#endif