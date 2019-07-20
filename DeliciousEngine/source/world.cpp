#include "world.h"

#include <algorithm>

#include "resources.h"
#include "screen.h"
#include "input.h"
#include "dmath.h"
#include "material.h"

bool World::load() {
	std::memset(entity_flags, false, sizeof(bool) * MAX_ENTITIES);
	return true;
}

bool World::start() {
	return load_test();
}

bool World::free() {
	return true;
}

//@Temp
bool World::load_test() {
	
	Entity* cube = make_entity("cube");
	Mesh* m = resources.fetch_mesh("primitive.cube");
	Texture * tex = resources.load_texture("res/tile.png", "tex");
	Shader * s = resources.load_shader("res/default.glsl");
	Material * mat = resources.make_material("default", s);
	mat->set_texture("diffuse", tex);
	cube->get_renderer().set_mesh(m);
	cube->get_renderer().set_material(mat);
	cube->set_logic([](Entity* e) {e->get_transform().rotate(0.0, 1.0, 2.0); });

	screen.get_camera()->transform_matrix() = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
	using namespace std::chrono;
	load_time = steady_clock::now();
	current_time = load_time;

	return true;
}

Entity* World::make_entity(std::string name) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (!entity_flags[i]) {
			entity_pool[i] = Entity(name);
			entity_flags[i] = true;
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
		if (entity_flags[i])
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
		if (!entity_flags[i])
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
