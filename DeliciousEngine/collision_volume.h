#ifndef DELICIOUS_COLLISION_VOLUME_H
#define DELICIOUS_COLLISION_VOLUME_H

#include <glm/glm.hpp>

class Entity;

//@TODO: For now just have every collision volume as a bounding sphere.
class SphereCollider {
public:
	SphereCollider();
	SphereCollider(Entity* e, float r = 1.0f, glm::vec3 off = glm::vec3(0.0f));

	bool intersect(SphereCollider* col);

	float get_radius();
	glm::vec3 get_offset();

	void set_radius(float value);
	void set_offset(glm::vec3 value);

	Entity* get_entity();
	void    set_entity(Entity* ref);
private:
	Entity* entity;
	glm::vec3 offset;
	float radius;
};

#endif