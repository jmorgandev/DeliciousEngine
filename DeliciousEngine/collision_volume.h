#ifndef DELICIOUS_COLLISION_VOLUME_H
#define DELICIOUS_COLLISION_VOLUME_H

#include <glm/glm.hpp>

//@TODO: For now just have every collision volume as a bounding sphere.
struct CollisionVolume {
	class Entity* entity;
	float radius;
};

typedef CollisionVolume SphereCollider;

#endif