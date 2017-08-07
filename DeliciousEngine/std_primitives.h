#ifndef DELICIOUS_PRIMITIVES_H
#define DELICIOUS_PRIMITIVES_H

#include "mesh_data.h"
#include <string>

float triangle_shit[] = {
	0.55, -0.25, 0.5, 1.0,
	-0.25, -0.25, 0.5, 1.0,
	-0.25, 0.25, 0.5, 1.0
};

const GLchar* vertex_shader_source[] = {
	"#version 450 core\n"
	"layout (location = 0) in vec4 position;"
	"void main(void) {"
	"	gl_Position = position;"
	"}"
};

const GLchar* fragment_shader_source[] = {
	"#version 450 core\n"
	"out vec4 color;"
	"void main(void) {"
	//"	color = texelFetch(s, ivec2(gl_FragCoord.xy), 0);"
	"	color = vec4(1.0, 0.5, 1.0, 1.0);"
	"}"
};

#endif