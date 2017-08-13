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
	pixel_width = 1.0f / screen_w;
	pixel_height = 1.0f / screen_h;

	render_width = (font->cell_width * pixel_width) * 2.0f;
	render_height = (font->cell_height * pixel_height) * 2.0f;

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

void FontRenderer::draw_char(char c, int pixel_x, int pixel_y) {
	int off = (c - font->char_offset);
	int column = off % font->cell_columns;
	int row = off / font->cell_columns;

	float texcell_x = column * texcell_width;
	float texcell_y = row * texcell_height;

	//float render_x = ((float)pixel_x / last_screen_width) * 2.0f - 1.0f;
	//float render_y = -(((float)pixel_y / last_screen_height) * 2.0f - 1.0f);

	float render_x = (pixel_x * pixel_width) * 2.0f - 1.0f;
	float render_y = (pixel_y * pixel_height) * 2.0f - 1.0f;

	glUniform2f(2, render_x, -render_y);
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