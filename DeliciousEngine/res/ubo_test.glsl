#version 430 core

#scope vertex
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

uniform matrices {
	mat4 transform;
	mat4 view;
	mat4 projection
}

out vec2 uv;

void main(void) {
	gl_Position = projection * view * transform * vec4(position, 1.0);
	uv = texcoord;
}

#scope fragment
in vec2 uv;
out vec4 color;

uniform sampler2D textureSampler;
uniform float highlight;

void main(void) {
	color = mix(texture(textureSampler, uv), vec4(1.0), highlight)
}