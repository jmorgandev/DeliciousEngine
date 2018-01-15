#ifndef DELICIOUS_WORLD_H
#define DELICIOUS_WORLD_H

#include "system_ref.h"
#include <vector>
#include <list>

#include "system_var.h"
#include "entity.h"
#include "x_renderer.h"

class World {
public:
	bool init(System_Ref sys);
	void clean_exit();

	bool load_test();

	void update();
	void draw();

	Entity* get_entity(uint index);
	
	Entity* find_entity(std::string name);
	Entity* find_entities(std::string name);

	Entity* create_entity(std::string name = "NewEntity");

	Entity* copy_entity(Entity* ent);

	Entity* clone_entity(Entity* ent, glm::vec3 pos);
	Entity* clone_entity(Entity* ent, glm::vec3 pos, glm::quat rot);
	Entity* clone_entity(Entity* ent, glm::vec3 pos, glm::vec3 scale);
	Entity* clone_entity(Entity* ent, glm::vec3 pos, glm::quat rot, glm::vec3 scale);
	Entity* clone_entity(Entity* ent, Transform* tfm);

	void destroy_entity(Entity* ent);

	bool collision(Entity* a, Entity* b);
private:
	System_Ref system;

	std::list<Entity> entities;

	//@TEMP
	void do_camera();
	Material* default_material;
	MaterialX test_mat;
	MeshRendererX test_rend;
};

#endif