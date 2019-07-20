#include "world.h"

#include <algorithm>

#include "resources.h"
#include "screen.h"
#include "input.h"
#include "dmath.h"
#include "scripting.h"
#include "material.h"

bool World::init() {
	std::memset(entity_flag, false, sizeof(bool) * MAX_ENTITIES);

	return true;
}

//@Temp
bool World::load_test() {
	
	screen.get_camera()->transform_matrix() = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
	using namespace std::chrono;
	load_time = steady_clock::now();
	current_time = load_time;

	return true;
}

void World::clean_exit() {

}

Entity* World::make_entity(std::string name) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (!entity_flag[i]) {
			entity_pool[i] = Entity(name);
			entity_flag[i] = true;
			return &entity_pool[i];
		}
	}
	return nullptr;
}

void World::update() {
	using namespace std::chrono;
	current_time = steady_clock::now();
	//@Temp: Should just be iterating through entities and calling scripts, this is just test logic for now...
	do_camera();

	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (entity_flag[i])
			entity_pool[i].update();
	}
}

void World::draw() {
	//@Temp
	//Entity* first = get_entity(0);
	//Entity* second = get_entity(1);
	//Entity* third = get_entity(2);
	//
	Camera* cam = screen.get_camera();
	//glm::mat4 transform_a = first->get_transform().get_matrix();
	//glm::mat4 transform_b = second->get_transform().get_matrix();
	//glm::mat4 transform_c = third->get_transform().get_matrix();
	//
	//glm::mat4 projection = cam->projection_matrix();
	//glm::mat4 view = cam->view_matrix();
	glm::mat4 projection = cam->projection_matrix();
	glm::mat4 view = cam->view_matrix();
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (!entity_flag[i])
			continue;

		Entity& entity = entity_pool[i];
		glm::mat4 transform = entity.get_transform().get_matrix();
		Material* mat = entity.get_renderer().get_material();
		if (mat == nullptr) continue;
		mat->set_matrix("projection", projection);
		mat->set_matrix("view", view);
		mat->set_matrix("transform", transform);
		entity.get_renderer().draw();
	}
}

void World::do_camera() {
	//@Temp: Until scripting

	Camera* cam = screen.get_camera();

	glm::vec3 cam_direction = { 0.0f, 0.0f, 0.0f };
	glm::vec3 cam_axis = { 0.0f, 1.0f, 0.0f };
	float cam_angle = 0.0f;

	if (input.get_key(SDLK_w)) cam_direction.z = -1.0f;
	if (input.get_key(SDLK_a)) cam_direction.x = -1.0f;
	if (input.get_key(SDLK_d)) cam_direction.x = 1.0f;
	if (input.get_key(SDLK_s)) cam_direction.z = 1.0f;

	if (glm::length(cam_direction) > 0.0f) {
		cam_direction = glm::normalize(cam_direction);
	}

	cam->transform_matrix() = glm::translate(cam->transform_matrix(), cam_direction * 0.05f);
	cam->transform_matrix() = glm::rotate(cam->transform_matrix(), -glm::radians(cam_angle), cam_axis);
}
