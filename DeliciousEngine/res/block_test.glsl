#version 430 core

#scope vertex

in vec3 position;
in vec3 normal;
in vec2 texcoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;

void main(void) {
	gl_Position = projection * view * transform * vec4(position, 1.0);
	uv = texcoord;
}

#scope fragment

in vec2 uv;
uniform sampler2D textureSampler;
out vec4 color;

void main(void) {
	color = texture(textureSampler, uv);
}

