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
}
Font* FontRenderer::get_font() {
	return font;
}

void FontRenderer::begin() {
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
	float xo = (off % font->cell_columns);
	float yo = (off / font->cell_columns);
	int yo2 = (yo / font->cell_rows);

	float cell_w = (float)font->cell_width / (font->texture->width); //- (1/font->texture->width);
	float cell_h = (float)font->cell_height / (font->texture->height);

	float cell_x = ((float)xo * cell_w);
	float cell_y = ((float)yo * cell_h);

	glUniform2f(2, x, y);
	glUniform2f(3, cell_x, cell_y);
	glUniform2f(4, cell_w, cell_h);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void FontRenderer::draw_string(const char* str, float x, float y) {
	float adv_x = ((float)font->cell_width / 1280);
	for (int i = 0; *str; i++) {
		float pos = x + ((float)i * 1.0f);
		draw_char(*str++, pos, y);
	}
}