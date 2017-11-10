#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

Transform::Transform() {
	position = { 0.0f, 0.0f, 0.0f };
	rotation = glm::quat();
	scale = { 1.0f, 1.0f, 1.0f };
	rebuild_matrix = true;
}

glm::vec3 Transform::get_position() {
	return position;
}
glm::quat Transform::get_rotation() {
	return rotation;
}
glm::vec3 Transform::get_euler_angles() {
	//@TODO: Implement
	return glm::vec3(1.0f);
}
glm::vec3 Transform::get_scale() {
	return scale;
}
glm::mat4 Transform::get_matrix() {
	if (rebuild_matrix == true) {
		//@TODO: recalculate the matrix from member variables
		glm::mat4 t_matrix = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 r_matrix = glm::mat4_cast(rotation);
		glm::mat4 s_matrix = glm::scale(glm::mat4(1.0f), scale);

		matrix = t_matrix * r_matrix * s_matrix;
		rebuild_matrix = false;
	}
	return matrix;
}

void Transform::set_position(const glm::vec3& new_position) {
	position = new_position;
	rebuild_matrix = true;
}
void Transform::set_position(const float& x, const float& y, const float& z) {
	position.x = x;
	position.y = y;
	position.z = z;
	rebuild_matrix = true;
}
void Transform::set_rotation(const glm::quat& new_rotation) {
	rotation = new_rotation;
	rebuild_matrix = true;
}
void Transform::set_euler_angles(const glm::vec3& angles) {
	//@TODO: Implement.
}
void Transform::set_scale(const glm::vec3& new_scale) {
	scale = new_scale;
	rebuild_matrix = true;
}

void Transform::translate(const glm::vec3& vector) {
	position += vector;
	rebuild_matrix = true;
}
void Transform::translate(const float& x, const float& y, const float& z) {
	position.x += x;
	position.y += y;
	position.z += z;
	rebuild_matrix = true;
}

void Transform::rotate(const glm::quat& quaternion) {
	rotation += quaternion;
	rebuild_matrix = true;
}

void Transform::rotate(const glm::vec3& euler_angles) {
	// IS THIS CORRECT?
	glm::vec3 euler_radians = glm::radians(euler_angles);
	rotation = glm::rotate(rotation, glm::length(euler_radians), euler_radians);
	rebuild_matrix = true;
}

void Transform::rotate(const float& angle, const glm::vec3& axis) {
	rotation = glm::rotate(rotation, glm::radians(angle), axis);
	rebuild_matrix = true;
}

void Transform::rotate(const float& x, const float& y, const float& z) {
	rotate({ x,y,z });
}