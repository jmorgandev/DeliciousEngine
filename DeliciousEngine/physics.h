#ifndef DELICIOUS_PHYSICS_H
#define DELICIOUS_PHYSICS_H

#include "system_ref.h"
#include <vector>

#include "collision_volume.h"

struct Collision {
	Entity* a;
	Entity* b;
};

class Physics {
public:
	bool init(System_Ref sys);

	void update();

	void add_collider(SphereCollider* col);

	//@TEMP
	bool intersection(Entity* first, Entity* second);
private:
	std::vector<SphereCollider*> colliders;
	std::vector<Collision> collision_list;

	bool collision_exists(Entity* a, Entity* b);

	System_Ref system;
};

#endif