#version 450 core

#scope vertex
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out vec2 uv;

void main(void) {
	gl_Position = vec4(position, 1.0);
	uv = texcoord;
}

#scope fragment
in vec2 uv;
uniform sampler2D textureSampler;
out vec4 color;

void main(void) {
	color = texture(textureSampler, uv);
}

