#include "physics.h"
#include "entity.h"

bool Physics::collision_exists(Entity* a, Entity* b) {
	for (const auto& collision : collision_list) {
		if (collision.a == a && collision.b == b ||
			collision.a == b && collision.b == a) {
			return true;
		}
	}
	return false;
}

bool Physics::init() {
	return true;
}

void Physics::update() {
	for (auto i = colliders.begin(); i != colliders.end(); ++i) {
		for (auto j = colliders.begin(); j != colliders.end(); ++j) {
			if (i == j) continue;

			Entity* a = (*i)->get_entity();
			Entity* b = (*j)->get_entity();

			if (!collision_exists(a, b)) {
				collision_list.push_back({ a, b });
			}
		}
	}
}

//@TEMP
bool Physics::intersection(Entity* first, Entity* second) {
	return collision_exists(first, second);
}

void Physics::add_collider(SphereCollider* col) {
	colliders.push_back(col);
}

