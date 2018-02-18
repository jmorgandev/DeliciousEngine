#ifndef DELICIOUS_TRANSFORM_H
#define DELICIOUS_TRANSFORM_H

#include <glm.hpp>
#include <gtc/quaternion.hpp>

class Transform {
public:
	Transform();

	glm::vec3 get_position();
	glm::quat get_rotation();
	glm::vec3 get_euler_angles();
	glm::vec3 get_scale();
	glm::mat4 get_matrix();

	void set_position(const glm::vec3& new_position);
	void set_position(const float& x, const float& y, const float& z);
	void set_rotation(const glm::quat& new_rotation);
	void set_euler_angles(const glm::vec3& new_angles);
	void set_scale(const glm::vec3& new_scale);

	void translate(const glm::vec3& vector);
	void translate(const float& x, const float& y, const float& z);

	void rotate(const glm::quat& quaternion);
	void rotate(const glm::vec3& euler_angles);
	void rotate(const float& angle, const glm::vec3& axis);
	void rotate(const float& x, const float& y, const float& z);
private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 matrix;
	
	bool rebuild_matrix;
};

#endif