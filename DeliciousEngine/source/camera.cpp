#include "camera.h"
#include "screen.h"
#include <gtc/matrix_transform.hpp>

Camera::Camera() {
	transform = view = projection = glm::mat4(1.0f);
	fov = 60.0f;
	nearclip = 0.1f;
	farclip = 1000.0f;
	ortho = false;
}

void Camera::update_projection() {
	if (ortho)
		projection = glm::ortho(0.0f, (float)screen.width(), (float)screen.height(), 0.0f);
	else
		projection = glm::perspective(glm::radians(fov), screen.aspect_ratio(), nearclip, farclip);
	view = glm::inverse(transform);
}

void Camera::look_at(glm::vec3 pos) {
	//@Todo: Find a way to override the call to Camera::update, which overrides the view matrix.
	glm::vec3 camera_pos = glm::vec3(transform[3]);
	transform = glm::inverse(glm::lookAt(camera_pos, pos, { 0.0f, 1.0f, 0.0f }));
}