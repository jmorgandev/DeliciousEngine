#ifndef DELICIOUS_PRIMITIVES_H
#define DELICIOUS_PRIMITIVES_H

#include "mesh_data.h"
#include <string>

MeshData std_triangle
{
	{
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.0f, 0.5f, 0.0f)
	},
	{
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	},
	{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.5f, 1.0f),
	}
};

#endif