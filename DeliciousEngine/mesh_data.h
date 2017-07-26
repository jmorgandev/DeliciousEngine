#ifndef DELICIOUS_MESH_DATA_H
#define DELICIOUS_MESH_DATA_H

#include <vector>
#include <glm/glm.hpp>

struct MeshData {
	std::vector<glm::vec3> vertex_data;
	std::vector<glm::vec3> normal_data;
	std::vector<glm::vec3> color_data;
	std::vector<glm::vec2> texcoord_data;
	std::vector<unsigned> index_data;
};

#endif