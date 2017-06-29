#ifndef DELICIOUS_MESH_DATA_H
#define DELICIOUS_MESH_DATA_H

#include <vector>

struct MeshData {
	std::vector<float> vertex_data;
	std::vector<float> normal_data;
	std::vector<float> color_data;
	std::vector<float> texcoord_data;
};

#endif