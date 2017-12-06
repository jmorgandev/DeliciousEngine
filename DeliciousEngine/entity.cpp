#include "entity.h"

Entity::Entity() {
	collider = { this, 0.6f };
}

Entity::Entity(uint new_id) : Entity() {
	id = new_id;
}

Transform* Entity::get_transform() {
	return &transform;
}

MeshRenderer* Entity::get_renderer() {
	return &renderer;
}

SphereCollider* Entity::get_collider() {
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

uint Entity::get_id() {
	return id;
}

void Entity::set_id(uint new_id) {
	id = new_id;
}