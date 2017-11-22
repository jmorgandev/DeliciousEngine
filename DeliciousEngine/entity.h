#ifndef DELICIOUS_ENTITY_H
#define DELICIOUS_ENTITY_H

#include "transform.h"
#include "mesh_renderer.h"
#include "collision_volume.h"

//@TODO, @SPEED: Consider SoA vs AoS. Data-oriented design
// Should entities have individual MeshRenderers or just meshes that can be sent
// to a global MeshRenderer?

class Entity {
public:
	Entity();

	Transform* get_transform();
	MeshRenderer* get_renderer();
	CollisionVolume* get_collider();

	bool colliding_with(Entity* ent);
private:
	Transform transform;
	CollisionVolume collider;

	MeshRenderer renderer;
};

#endif