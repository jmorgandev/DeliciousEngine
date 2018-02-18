#include "camera.h"
#include <gtc/matrix_transform.hpp>

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

void Camera::look_at(glm::vec3 pos) {
	//@TODO: Find a way to override the call to Camera::update, which overrides the view matrix.
	glm::vec3 camera_pos = glm::vec3(transform[3]);
	transform = glm::inverse(glm::lookAt(camera_pos, pos, { 0.0f, 1.0f, 0.0f }));
}