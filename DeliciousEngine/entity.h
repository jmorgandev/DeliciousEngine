#ifndef DELICIOUS_ENTITY_H
#define DELICIOUS_ENTITY_H

#include "transform.h"
#include "mesh_renderer.h"

//@TODO, @SPEED: Consider SoA vs AoS. Data-oriented design
// Should entities have individual MeshRenderers or just meshes that can be sent
// to a global MeshRenderer?

class Entity {
public:
	Entity() {}

	Transform* get_transform() { return &transform; }
	MeshRenderer* get_renderer() { return &renderer; }

private:
	Transform transform;
	MeshRenderer renderer;
};

#endif