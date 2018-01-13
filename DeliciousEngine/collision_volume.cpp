#include "collision_volume.h"
#include "entity.h"

SphereCollider::SphereCollider() {
	entity = nullptr;
	radius = 1.0f;
	offset = glm::vec3(0.0f);
}
SphereCollider::SphereCollider(Entity* e, float r, glm::vec3 off) {
	entity = e;
	radius = r;
	offset = off;
}

bool SphereCollider::intersect(SphereCollider* col) {
	glm::vec3 pos_a = col->entity->get_transform()->get_position() + col->get_offset();
	glm::vec3 pos_b = entity->get_transform()->get_position() + offset;

	glm::vec3 diff = pos_b - pos_a;
	float total_radius = radius + col->radius;

	return (glm::length(diff) < total_radius);
}

float SphereCollider::get_radius() {
	return radius;
}
void SphereCollider::set_radius(float value) {
	radius = value;
}

glm::vec3 SphereCollider::get_offset() {
	return offset;
}
void SphereCollider::set_offset(glm::vec3 value) {
	offset = value;
}

Entity* SphereCollider::get_entity() {
	return entity;
}
void SphereCollider::set_entity(Entity* ref) {
	entity = ref;
}

