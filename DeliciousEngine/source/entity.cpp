#include "entity.h"

Entity::Entity(std::string name) {
	this->name = name;
	collider = { this, 0.6f };
}
Entity::Entity(const Entity& e) {
	name = e.name;
	transform = e.transform;
	renderer = e.renderer;
	collider = e.collider;
	collider.set_entity(this);
}

void Entity::set_transform(Transform* value) {
	transform = *value;
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