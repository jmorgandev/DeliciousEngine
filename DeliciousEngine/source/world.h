#ifndef DELICIOUS_WORLD_H
#define DELICIOUS_WORLD_H

#include <vector>
#include <array>
#include "module.h"
#include <list>
#include "build_settings.h"
#include "dtypes.h"
#include "entity.h"

class World : public Module {
public:
	using Module::Module;

	bool load() override;
	bool start() override;
	bool free() override;

	void update();
	void draw();

	Entity* make_entity(std::string name = "entity");
private:
	Entity entity_pool[MAX_ENTITIES];
	bool  entity_flags[MAX_ENTITIES];
	//@Temp
	void do_camera();
	bool load_test();
};

#endif