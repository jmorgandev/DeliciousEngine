#ifndef DELICIOUS_WORLD_H
#define DELICIOUS_WORLD_H

#include <list>
#include <chrono>
#include "build_settings.h"
#include "system_var.h"
#include "entity.h"
#include "dtypes.h"

using time_point = std::chrono::time_point<std::chrono::steady_clock>;
using duration = std::chrono::duration<double>;

class World {
public:
	bool init();
	void clean_exit();

	bool load_test();

	void update();
	void draw();

	double get_time() {
		return std::chrono::duration_cast<duration>(current_time - load_time).count();
	}

	Entity* make_entity(std::string name = "entity");
private:
	Entity entity_pool[MAX_ENTITIES];
	bool   entity_flag[MAX_ENTITIES];
	time_point load_time;
	time_point current_time;
	//@Temp
	void do_camera();
};
extern World world;

#endif