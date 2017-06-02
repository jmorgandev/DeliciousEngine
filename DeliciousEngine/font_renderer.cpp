#include "font_renderer.h"

#include "font.h"

FontRenderer::FontRenderer(Font* font_in) {
	font = font_in;
}

void FontRenderer::set_font(Font* font_in) {
	font = font_in;
}
Font* FontRenderer::get_font() {
	return font;
}

void draw_char(char c, float x, float y) {

}