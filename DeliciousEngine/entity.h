#ifndef DELICIOUS_ENTITY_H
#define DELICIOUS_ENTITY_H

#include "transform.h"
#include "mesh_renderer.h"

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