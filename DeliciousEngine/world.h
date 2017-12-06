#ifndef DELICIOUS_WORLD_H
#define DELICIOUS_WORLD_H

#include "system_ref.h"
#include <vector>

#include "system_var.h"
#include "entity.h"

class World {
public:
	bool init(System_Ref sys);
	void clean_exit();

	bool load_test();

	void update();
	void draw();

	Entity* GetEntityByIndex(uint i);
	Entity* GetEntityByID(uint id);

	Entity* CreateEntity();
	Entity* AddEntity(Entity ent);
private:
	System_Ref system;

	Entity entity_a;
	Entity entity_b;

	std::vector<Entity> entities;

	//@TEMP
	void do_camera();
	Material* default_material;
};

#endif