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
private:
	System_Ref system;

	Entity entity_a;
	Entity entity_b;

	//@TEMP
	void do_camera();
	Material* default_material;
};

#endif