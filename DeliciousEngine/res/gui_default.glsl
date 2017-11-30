#version 430 core

#scope vertex

in vec2 position;
in vec2 texcoord;
out vec2 uv;

uniform mat4 projection;

void main(void) {
	gl_Position = projection * vec4(position, 0.0, 1.0);
}

#scope fragment

out pixel;

uniform vec4 color;
uniform sampler2D diffuse;

void main(void) {
	pixel = color * texture(diffuse, uv);
}