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
	Entity(uint new_id = 0);
	virtual ~Entity() {}

	Transform* get_transform();
	MeshRenderer* get_renderer();
	SphereCollider* get_collider();

	uint get_id();
	void set_id(uint value);
private:
	uint id;

	Transform transform;
	SphereCollider collider;
	MeshRenderer renderer;
};

#endif