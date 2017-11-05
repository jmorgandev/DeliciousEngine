#ifndef DELICIOUS_CAMERA_H
#define DELICIOUS_CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	Camera();

	void calculate_projection(float fov, float aspect_ratio);

	glm::mat4& get_transform()		   { return matrix_transform;  }
	glm::mat4& get_view_matrix()	   { return matrix_view;       }
	glm::mat4& get_projection_matrix() { return matrix_projection; }

private:
	glm::mat4 matrix_transform;
	glm::mat4 matrix_view;
	glm::mat4 matrix_projection;
};

#endif