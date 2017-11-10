#ifndef DELICIOUS_WORLD_H
#define DELICIOUS_WORLD_H

#include "system_ref.h"
#include "system_var.h"
#include "entity.h"

class World {
public:
	bool init(System_Ref sys);
	void clean_exit();

	void load_test();

	void update();
	void draw();
private:
	System_Ref system;

	Entity test_entity;
};

#endif