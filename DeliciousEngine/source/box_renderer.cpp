#include "box_renderer.h"
#include "shader.h"
#include "mesh.h"

BoxRenderer::BoxRenderer() {
	box_vao = 0;
}

void BoxRenderer::set_vao(GLuint vao) {
	box_vao = vao;
}

void BoxRenderer::set_shader(Shader* s) {
	shader = s;

	uniform_translation = glGetUniformLocation(shader->id, "translation");
	uniform_scale = glGetUniformLocation(shader->id, "scale");
	uniform_color = glGetUniformLocation(shader->id, "color");
}

void BoxRenderer::begin(int screen_w, int screen_h) {
	pixel_width = 1.0f / screen_w;
	pixel_height = 1.0f / screen_h;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(shader->id);
	glBindVertexArray(box_vao);
}

void BoxRenderer::end() {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void BoxRenderer::draw(int from_x, int from_y, int to_x, int to_y, glm::vec4 color) {
	float render_x = (from_x * pixel_width) * 2.0f - 1.0f;
	float render_y = (from_y * pixel_height) * 2.0f - 1.0f;

	float scale_x = ((to_x - from_x) * pixel_width) * 2.0f;
	float scale_y = ((to_y - from_y) * pixel_height) * 2.0f;

	glUniform2f(uniform_translation, render_x, -render_y);
	glUniform2f(uniform_scale, scale_x, scale_y);
	glUniform4f(uniform_color, color.r, color.g, color.b, color.a);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}