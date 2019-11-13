#ifndef DELICIOUS_MESH_DATA_H
#define DELICIOUS_MESH_DATA_H

#include <vector>
#include <tuple>
#include <vec3.hpp>
#include <vec2.hpp>
#include <vec4.hpp>

struct MeshData {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
};

struct Vertex_VNCT {
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec4 color;
	glm::vec2 texcoord;
};

#endif