#ifndef DELICIOUS_MESH_DATA_H
#define DELICIOUS_MESH_DATA_H

#include <vector>
#include <glm/glm.hpp>

struct MeshData {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
};

#endif