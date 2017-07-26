#ifndef DELICIOUS_PRIMITIVES_H
#define DELICIOUS_PRIMITIVES_H

#include "mesh_data.h"

MeshData primitive_quad{
	/* VERTEX_DATA */{
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.0f)
	},
	/* NORMAL DATA */ {
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	},
	/* COLOR_DATA */ {},
	/* TEXCOORD_DATA*/ {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	},
	/* INDEX_DATA */ {
		0, 1, 2,			//triangle_0
		0, 2, 3				//triangle_1
	}
};

#endif