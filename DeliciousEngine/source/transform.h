#ifndef DELICIOUS_TRANSFORM_H
#define DELICIOUS_TRANSFORM_H

#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtc/quaternion.hpp>

class Transform {
public:
	Transform();

	glm::vec3 get_position() const { return position; }
	glm::quat get_rotation() const { return rotation; }
	glm::vec3 get_euler_angles() const { return glm::eulerAngles(rotation); }
	glm::vec3 get_scale() const { return scale; }
	glm::mat4 get_matrix();

	void set_position(const glm::vec3& position) { 
		this->position = position; 
		rebuild_matrix = true;
	}
	void set_position(const float& x, const float& y, const float& z) {
		position.x = x;
		position.y = y;
		position.z = z;
		rebuild_matrix = true;
	}

	void set_rotation(const glm::quat& rotation) { 
		this->rotation = rotation;
		rebuild_matrix = true; 
	}
	void set_rotation(const glm::vec3& angles) {
		glm::vec3 radians = glm::radians(angles);
		rotation = glm::quat(radians);
		rebuild_matrix = true;
	}

	void set_scale(const glm::vec3 scale) { 
		this->scale = scale; 
		rebuild_matrix = true;
	}

	void translate(const glm::vec3& vector) { 
		position += vector; 
		rebuild_matrix = true;
	}
	void translate(const float& x, const float& y, const float& z) {
		position.x += x;
		position.y += y;
		position.z += z;
		rebuild_matrix = true;
	}

	void rotate(const glm::quat& quaternion) { 
		rotation = rotation * quaternion; 
		rebuild_matrix = true; 
	}
	void rotate(const glm::vec3& angles) {
		glm::vec3 radians = glm::radians(angles);
		rotation = rotation * glm::quat(radians);
		rebuild_matrix = true;
	}
	void rotate(const float& angle, const glm::vec3 axis) {
		rotation = glm::rotate(rotation, glm::radians(angle), axis);
		rebuild_matrix = true;
	}
	void rotate(const float& x, const float& y, const float& z) {
		glm::vec3 radians = glm::radians(glm::vec3(x, y, z));
		rotation = rotation * glm::quat(radians);
		rebuild_matrix = true;
	}
private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 matrix;
	
	bool rebuild_matrix;
};

#endif