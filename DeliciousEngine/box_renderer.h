#ifndef DELICIOUS_BOX_RENDERER_H
#define DELICIOUS_BOX_RENDERER_H

#include "dtypes.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Shader;
class BoxRenderer {
public:
	BoxRenderer();

	void begin(int screen_w, int screen_h);
	void draw(int from_x, int from_y, int to_x, int to_y, glm::vec4 color);
	void end();

	void set_vao(GLuint vao);
	void set_shader(Shader* s);
private:
	GLuint box_vao;
	Shader* shader;
	float pixel_width;
	float pixel_height;
};

#endif