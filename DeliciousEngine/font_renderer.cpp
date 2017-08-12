#include "font_renderer.h"

#include <GL/glew.h>
#include "font.h"
#include "shader.h"
#include "texture.h"

FontRenderer::FontRenderer() {
	font = NULL;
}

void FontRenderer::set_font(Font* font_in) {
	font = font_in;
	texcell_width = (float)font->cell_width / font->texture->width;
	texcell_height = (float)font->cell_height / font->texture->height;
}
Font* FontRenderer::get_font() {
	return font;
}

void FontRenderer::begin(int screen_w, int screen_h) {
	render_width = (float)font->cell_width / screen_w;
	render_height = (float)font->cell_height / screen_h;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(font->shader->id);
	glBindTexture(GL_TEXTURE_2D, font->texture->id);
	glBindVertexArray(font->gui_vao);
}

void FontRenderer::end() {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void FontRenderer::draw_char(char c, float x, float y) {
	int off = (c - font->char_offset);
	int column = off % font->cell_columns;
	int row = off / font->cell_columns;

	float texcell_x = (float)column * texcell_width;
	float texcell_y = (float)row * texcell_height;

	glUniform2f(2, x, y);
	glUniform2f(3, texcell_x, texcell_y);
	glUniform2f(4, texcell_width, texcell_height);
	glUniform2f(5, render_width, render_height);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void FontRenderer::draw_string(const char* str, float x, float y) {
	//float adv_x = ((float)font->cell_width / 1280);
	//for (int i = 0; *str; i++) {
	//	float pos = x + ((float)i * 1.0f);
	//	draw_char(*str++, pos, y);
	//}
}