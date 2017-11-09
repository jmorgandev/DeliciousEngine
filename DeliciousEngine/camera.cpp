#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	transform = view = projection = glm::mat4(1.0f);
}

void Camera::init(system_var* fov_var, system_var* aspect_var) {
	field_of_view = fov_var;
	aspect_ratio = aspect_var;
}

void Camera::update() {
	projection = glm::perspective(glm::radians(field_of_view->as_float), aspect_ratio->as_float, 0.1f, 1000.0f);
	view = glm::inverse(transform);
}