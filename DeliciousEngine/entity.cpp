#include "entity.h"

Entity::Entity(uint new_id) {
	collider = { this, 0.6f };
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

uint Entity::get_id() {
	return id;
}

void Entity::set_id(uint new_id) {
	id = new_id;
}