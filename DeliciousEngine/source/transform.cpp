#include "transform.h"

#include <gtc/matrix_transform.hpp>

Transform::Transform() {
	position = glm::vec3(0.0f);
	rotation = glm::quat(glm::vec3(0.0f));
	scale = glm::vec3(1.0f);
	rebuild_matrix = true;
}

glm::mat4 Transform::get_matrix() {
	if (rebuild_matrix == true) {
		glm::mat4 t_matrix = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 r_matrix = glm::mat4_cast(rotation);
		glm::mat4 s_matrix = glm::scale(glm::mat4(1.0f), scale);

		matrix = t_matrix * r_matrix * s_matrix;
		rebuild_matrix = false;
	}
	return matrix;
}