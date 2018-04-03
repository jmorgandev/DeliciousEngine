#ifndef DELICIOUS_ENTITY_H
#define DELICIOUS_ENTITY_H

#include <string>

#include "dtypes.h"
#include "transform.h"
#include "mesh_renderer.h"
#include <mat4x4.hpp>
#include <vec3.hpp>
#include <gtc/quaternion.hpp>

#include <sol.hpp>

//@Todo, @Speed: Consider SoA vs AoS. Data-oriented design
// Should entities have individual MeshRenderers or just meshes that can be sent
// to a global MeshRenderer?

class Entity {
public:
	Entity(std::string name = "Entity", uint tag = 0) : name(name), tag_index(tag) {}
	virtual ~Entity() {}

	void set_script(sol::this_state ts, sol::table script);

	std::string get_name() const { return name; }
	void set_name(const std::string& name) { this->name = name; }

	uint get_tag() const { return tag_index; }
	void set_tag(const uint& tag) { tag_index = tag; }

	Transform& get_transform() { return transform; }
	MeshRenderer& get_renderer() { return renderer; }
	
	virtual void load();
	virtual void begin();
	virtual void update();
	virtual void end();
protected:
	Transform transform;
	MeshRenderer renderer;

	std::string name;
	uint tag_index;

	sol::table lua_script;
};

#endif