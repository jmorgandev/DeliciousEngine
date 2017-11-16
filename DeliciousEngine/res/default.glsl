#version 430 core

#scope vertex
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;

void main(void) {
	//@TODO: Don't multiply matrices per vertex you mad-man
	//Precompute this on the CPU and store as MVP_matrix
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

