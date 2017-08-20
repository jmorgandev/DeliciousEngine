#include "dgl.h"
#include "dtypes.h"
#include "dcf.h"
#include "build_info.h"
#include <algorithm>

namespace dgl {

	bool compile(const GLuint& object, const std::string& source) {
		const GLchar* src = source.c_str();
		const GLint size = source.length();
		glShaderSource(object, 1, &src, NULL);
		glCompileShader(object);
		GLint status = 0;
		glGetShaderiv(object, GL_COMPILE_STATUS, &status);
		return (status != GL_FALSE);
	}

	bool compile(const GLuint& object, const GLchar* source) {
		const GLint size = dcf::str_len(source);
		glShaderSource(object, 1, &source, NULL);
		glCompileShader(object);
		GLint status = 0;
		glGetShaderiv(object, GL_COMPILE_STATUS, &status);
		return (status != GL_FALSE);
	}

	/*
	int validate_meshdata(MeshData& data) {
		uint32 triangle_count = 0;
		uint32 num_buffers = 3;
		// If there are no indicies, the vertex count must be divisible by 3
		if (data.indices.empty()) {
			if (data.vertices.size() % 3 != 0) {
				return 0;	//Should be the only failure case, we always need vertex positions.
			}
			// If there are no indices, auto generate them so that glDrawElements can be used.
			// @TODO make something that isn't as wasteful
			for (uint32 i = 0; i < data.vertices.size(); i++) {
				data.indices.push_back(i);
			}
			triangle_count = data.vertices.size() / 3;
		}
		else {
			// User defined indices must be divisible by 3 and any index value cannot exceed the size of the vertex count
			bool invalid_index = false;
			std::for_each(data.indices.begin(), data.indices.end(), [&invalid_index, &data](unsigned& i) {
				if (i >= data.vertices.size()) { invalid_index = true; return; }
			});

			if (data.indices.size() % 3 != 0 || invalid_index) {
				// Throw away user defined indices and replace with auto generated ones.
				data.indices.clear();
				for (uint32 i = 0; i < data.vertices.size(); i++) {
					data.indices.push_back(i);
				}
			}
			triangle_count = data.indices.size() / 3;
		}
		// The normal count must either match the vertex count, or the triangle count.
		if (data.normals.size() != data.vertices.size() && data.normals.size() != triangle_count) {
			// Throw away any user defined normals and replace with auto generated ones.
			data.normals.clear();
			for (int i = 0; i < data.indices.size(); i += 3) {
				const glm::vec3& a = data.vertices[data.indices[i]];
				const glm::vec3& b = data.vertices[data.indices[i + 1]];
				const glm::vec3& c = data.vertices[data.indices[i + 2]];
				glm::vec3 generated_face_normal = glm::normalize(glm::cross(c - a, b - a));
				data.normals.push_back(generated_face_normal);
				data.normals.push_back(generated_face_normal);
				data.normals.push_back(generated_face_normal);
			}
		}
		// The texcoord count must match the vertex count.
		if (data.texcoords.size() != data.vertices.size()) {
			data.texcoords.clear(); // Throw away the texcoord data.
		}
		else if(!data.texcoords.empty()) num_buffers++;
		// The color count must either match the vertex count, or the triangle count.
		if (data.colors.size() != data.vertices.size() && data.colors.size() != triangle_count) {
			data.colors.clear(); // Throw away the color data.
		}
		else if(!data.colors.empty()) num_buffers++;
		
		return num_buffers;
	}
	*/

	

}