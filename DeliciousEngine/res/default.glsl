#version 450 core

#scope vertex
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

void main(void) {
	gl_Position = vec4(position, 1.0);
}

#scope fragment
out vec4 color;

void main(void) {
	color = vec4(1.0, 1.0, 1.0, 1.0);
}

