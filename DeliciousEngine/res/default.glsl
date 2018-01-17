#version 430 core

uniform material {
	mat4 transform;
	mat4 view;
	mat4 projection;
	vec4 diffuse_tint;
	vec4 other_tint;
}

#scope vertex
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
out vec2 uv;

void main(void) {
	//@TODO: Don't multiply matrices per vertex you mad-man
	//Precompute this on the CPU and store as MVP_matrix
	gl_Position = projection * view * transform * vec4(position, 1.0);
	uv = texcoord;
}

#scope fragment
in vec2 uv;
out vec4 color;

uniform float mult;
uniform sampler2D diffuse;

void main(void) {
	vec4 final_tint = diffuse_tint + other_tint;
	color = mix(texture(diffuse, uv), final_tint, diffuse_tint.a * mult);
}

