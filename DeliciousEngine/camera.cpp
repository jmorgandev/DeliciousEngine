#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

//Camera::Camera() {
//	matrix_transform = matrix_view = matrix_projection = glm::mat4(1.0f);
//}

void Camera::calculate_projection(float fov, float aspect_ratio) {
	matrix_projection = glm::perspective(fov, aspect_ratio, 0.1f, 1000.0f);
}