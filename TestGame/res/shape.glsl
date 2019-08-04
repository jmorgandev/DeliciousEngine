#version 400 core

#scope vertex

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 uv;

uniform vec2 translation;
uniform vec2 scale;

void main(void) {
	gl_Position = vec4((position * scale) + translation, 0.0, 1.0f);
}

#scope fragment

out vec4 frag_color;

uniform vec4 color;

void main(void) {
	frag_color = color;
}