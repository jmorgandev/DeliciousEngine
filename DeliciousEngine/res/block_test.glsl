#scope vertex
in vec3 position;
in vec3 normal;
in vec2 texcoord;

uniform mvp_matrix;

out vec2 uv;

void main(void) {
	gl_Position = mvp_matrix * vec4(position, 1.0);
	uv = texcoord;
}

#scope fragment
in vec2 uv;
uniform sampler2D textureSampler;
out vec4 color;

void main(void) {
	color = texture(textureSampler, uv);
}

