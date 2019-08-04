#ifndef DELICIOUS_WORLD_H
#define DELICIOUS_WORLD_H

#include <vector>
#include <array>
#include "system.h"
#include <list>
#include <chrono>
#include "build_settings.h"
#include "system_var.h"
#include "dtypes.h"
#include "entity.h"

using time_point = std::chrono::time_point<std::chrono::steady_clock>;
using duration = std::chrono::duration<double>;

class World : public System {
public:
	World(DeliciousEngine& engine) : System(engine) {}
	World() = delete;

	bool load() override;
	bool start() override;
	bool free() override;

	void update();
	void draw();

	double get_time() {
		return std::chrono::duration_cast<duration>(current_time - load_time).count();
	}

	Entity* make_entity(std::string name = "entity");
private:
	Entity entity_pool[MAX_ENTITIES];
	bool  entity_flags[MAX_ENTITIES];
	time_point load_time;
	time_point current_time;
	//@Temp
	void do_camera();
	bool load_test();
};

#endif