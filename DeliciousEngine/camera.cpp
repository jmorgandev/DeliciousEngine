#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	transform = view = projection = glm::mat4(1.0f);
}