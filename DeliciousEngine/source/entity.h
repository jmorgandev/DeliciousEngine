#ifndef DELICIOUS_ENTITY_H
#define DELICIOUS_ENTITY_H

#include <string>

#include "dtypes.h"
#include "transform.h"
#include "mesh_renderer.h"
#include <mat4x4.hpp>
#include <vec3.hpp>
#include <gtc/quaternion.hpp>

//@Todo, @Speed: Consider SoA vs AoS. Data-oriented design
// Should entities have individual MeshRenderers or just meshes that can be sent
// to a global MeshRenderer?

class Entity {
public:
	Entity(std::string name = "new_entity", uint tag = 0) : name(name), tag_index(tag) {}
	virtual ~Entity() {}

	std::string get_name() const { return name; }
	void set_name(const std::string& name) { this->name = name; }

	uint get_tag() const { return tag_index; }
	void set_tag(const uint& tag) { tag_index = tag; }

	Transform& get_transform() { return transform; }
	MeshRenderer& get_renderer() { return renderer; }
protected:
	Transform transform;
	MeshRenderer renderer;

	std::string name;
	uint tag_index;
};

#endif