#version 430 core

#ifdef VERTEX

uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 texcoord;
out vec2 uv;

void main(void) {
	//@TODO: Don't multiply matrices per vertex you mad-man
	//Precompute this on the CPU and store as MVP_matrix
	gl_Position = projection * view * transform * vec4(position, 1.0);
	uv = texcoord;
}
#endif

#ifdef FRAGMENT
in vec2 uv;
out vec4 color;

uniform float mult;
uniform sampler2D diffuse;

void main(void) {
	//color = mix(texture(diffuse, uv), final_tint, diffuse_tint.a * mult);
	//color = diffuse_tint;
	color = texture(diffuse, uv);
}
#endif

