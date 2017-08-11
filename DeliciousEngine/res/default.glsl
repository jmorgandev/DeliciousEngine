#version 450 core

#scope vertex
layout (location = 0) in vec2 position;
//layout (location = 1) in vec3 normal;
layout (location = 1) in vec2 texcoord;

out vec2 uv;

void main(void) {
	gl_Position = vec4(position, 0.0, 1.0);
	uv = texcoord;
}

#scope fragment
in vec2 uv;
uniform sampler2D textureSampler;
out vec4 color;

void main(void) {
	//color = vec4(1.0, 1.0, 1.0, 1.0);
	color = texture(textureSampler, uv);
}

