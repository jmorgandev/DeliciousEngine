#version 430 core

#scope vertex

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 uv;

layout(location = 2) uniform vec2 translation;
layout(location = 3) uniform vec2 tex_offset;
layout(location = 4) uniform vec2 tex_scale;
layout(location = 5) uniform vec2 scale;

void main(void) {
	gl_Position = vec4((position * scale) + translation, 0.0, 1.0);
	uv = (texcoord * tex_scale) + tex_offset;
}

#scope fragment

in vec2 uv;

out vec4 color_result;

uniform sampler2D fontAtlas;

void main(void) {
	color_result = texture(fontAtlas, uv);
}