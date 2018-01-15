#version 430 core

#scope vertex
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
out vec2 uv;

uniform BlobSettings {
	vec4 innerColor;
	vec4 outerColor;
	float radiusInner;
	float radiusOuter;
	vec3 pos_offset;
};

void main() {
	uv = texcoord;
	gl_Position = vec4(position + pos_offset, 1.0);
}

#scope fragment
in vec2 uv;
out vec4 pixel;

uniform BlobSettings {
	vec4 innerColor;
	vec4 outerColor;
	float radiusInner;
	float radiusOuter;
	vec3 pos_offset;
};

uniform vec4 mixer;

void main() {
	float dx = uv.x - 0.5;
	float dy = uv.y - 0.5;
	float dist = sqrt(dx * dx + dy * dy);
	pixel = mix(
		innerColor, outerColor, 
		smoothstep(radiusInner, radiusOuter, dist)
	);
}