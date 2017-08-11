#version 450 core

#scope vertex

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texcoord;

out vec2 uv;

layout (location = 2) uniform vec2 translation;
layout (location = 3) uniform vec2 uv_offset;
layout (location = 4) uniform vec2 uv_scale;

void main(void) {
	gl_Position = vec4(position + translation, 0.0, 1.0);
	uv = (texcoord * uv_scale) + uv_offset;
}

#scope fragment

in vec2 uv;

out vec4 color_result;

layout (location = 5) uniform sampler2D fontAtlas;

void main(void) {
	color_result = texture(fontAtlas, uv);
}