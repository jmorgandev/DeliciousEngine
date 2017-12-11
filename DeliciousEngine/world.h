#ifndef DELICIOUS_WORLD_H
#define DELICIOUS_WORLD_H

#include "system_ref.h"
#include <vector>
#include <list>

#include "system_var.h"
#include "entity.h"

class World {
public:
	bool init(System_Ref sys);
	void clean_exit();

	bool load_test();

	void update();
	void draw();

	Entity* get_entity(uint id);

	Entity* create_entity(uint id = 0);
	Entity* clone_entity(Entity& src);
	Entity* clone_entity(Entity& src, uint id);
	void destroy_entity(uint id);
	void destroy_entity(Entity* ent);

	bool collision(Entity* a, Entity* b);
private:
	System_Ref system;

	//std::vector<Entity> entities;
	std::list<Entity> entities;

	//@TEMP
	void do_camera();
	Material* default_material;
};

#endif