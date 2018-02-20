#ifndef DELICIOUS_DEFAULT_SHADERS_H
#define DELICIOUS_DEFAULT_SHADERS_H
#pragma once

const char* gui_vert_shader =
"#version 430 core\n"
"uniform mat4 proj_matrix;"
"in vec2 position;"
"in vec2 texcoord;"
"in vec4 color;"
"out vec2 uv;"
"out vec4 frag_color;"
"void main() {"
"	uv = texcoord;"
"	frag_color = color;"
"	gl_Position = proj_matrix * vec4(position.xy, 0, 1);"
"}";

const char* gui_frag_shader =
"#version 430 core\n"
"uniform sampler2D tex;"
"in vec2 uv;"
"in vec4 frag_color;"
"out vec4 pixel;"
"void main() {"
"	pixel = frag_color * texture(tex, uv);"
"}";

#endif
