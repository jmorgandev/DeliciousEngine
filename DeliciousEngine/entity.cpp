#include "entity.h"

Entity::Entity() {
	collider = { this, 0.6f };
}

Transform* Entity::get_transform() {
	return &transform;
}

MeshRenderer* Entity::get_renderer() {
	return &renderer;
}

CollisionVolume* Entity::get_collider() {
	return &collider;
}

bool Entity::colliding_with(Entity* ent) {
	//@TEMP: Perform Bounding test here
	glm::vec3 pos = ent->get_transform()->get_position();
	glm::vec3 diff = transform.get_position() - pos;

	float sum_radius = ent->get_collider()->radius + collider.radius;

	if (glm::length(diff) < sum_radius) {
		return true;
	}
	else {
		return false;
	}
}