#include "font_renderer.h"

#include "font.h"

FontRenderer::FontRenderer() {
	font = NULL;
}

void FontRenderer::set_font(Font* font_in) {
	font = font_in;
}
Font* FontRenderer::get_font() {
	return font;
}

void FontRenderer::draw_char(char c, float x, float y) {
	if (font == NULL) return;
}