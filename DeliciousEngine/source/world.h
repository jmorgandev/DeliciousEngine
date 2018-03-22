#ifndef DELICIOUS_WORLD_H
#define DELICIOUS_WORLD_H

#include <list>
#include <chrono>

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

	Entity* get_entity(uint index);
	
	Entity* find_entity(std::string name);
	Entity* find_entities(std::string name);

	Entity* create_entity(std::string name = "NewEntity");

	Entity* copy_entity(Entity* ent);

	Entity* clone_entity(Entity* ent, glm::vec3 pos);
	Entity* clone_entity(Entity* ent, glm::vec3 pos, glm::quat rot);
	Entity* clone_entity(Entity* ent, glm::vec3 pos, glm::vec3 scale);
	Entity* clone_entity(Entity* ent, glm::vec3 pos, glm::quat rot, glm::vec3 scale);
	Entity* clone_entity(Entity* ent, Transform tfm);

	void destroy_entity(Entity* ent);

	bool collision(Entity* a, Entity* b);
private:
	std::list<Entity> entities;
	time_point load_time;
	time_point current_time;
	//@Temp
	void do_camera();
};
extern World world;

#endif