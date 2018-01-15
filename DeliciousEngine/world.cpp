#include "world.h"

#include "resources.h"
#include "screen.h"
#include "input.h"
#include "physics.h"
#include "dmath.h"

#include <algorithm>

#include "material.h"

bool World::init(System_Ref sys) {
	system = sys;
	return true;
}

//@TEMP
bool World::load_test() {
	Shader* test_shader = system.resources->load_shader("res/circle.glsl");
	assert(test_shader != nullptr);

	test_mat.set_shader(test_shader);
	//GLfloat col[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//test_mat.set_floatv("innerColor", col, 4);
	test_mat.set_vector4("outerColor", { 0.0f, 0.0f, 0.0f, 0.0f});
	test_mat.set_vector4("innerColor", { 1.0f, 1.0f, 0.75f, 1.0f });
	test_mat.set_float("radiusInner", 0.25f);
	test_mat.set_float("radiusOuter", 0.45f);

	Mesh* quad = system.resources->fetch_mesh("primitive.quad");
	test_rend.set(quad, &test_mat);	

	Texture* default_texture = system.resources->load_texture("res/tile.tga");
	Shader*  default_shader = system.resources->load_shader("res/default.glsl");
	if (default_texture == nullptr || default_shader == nullptr) {
		return false;
	}
	default_material = system.resources->make_material("default", default_texture, default_shader);
	Mesh* cube = system.resources->fetch_mesh("primitive.cube");

	//Entity first, second;
	Entity* first = create_entity("first");
	first->get_renderer()->set(cube, default_material);
	first->get_transform()->set_position(-1.0f, 0.0f, 0.0f);

	Entity* second = clone_entity(first, { 1.0f, 0.0f, 0.0f });
	second->name = "second";

	system.screen->get_camera()->transform_matrix() = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
	return true;
}

void World::clean_exit() {

}

void World::update() {
	//@TEMP: Should just be iterating through entities and calling scripts, this is just test logic for now...
	do_camera();

	Entity* first = get_entity(0);
	Entity* second = get_entity(1);

	glm::vec3 axis = glm::normalize(glm::vec3(0.0f, 2.0f, 0.0f));
	first->get_transform()->rotate(0.1f, axis);
	second->get_transform()->rotate(1.0f, axis);
	
	static float t = 0;
	t += 0.05f;
	float off = -0.7f;
	first->get_transform()->set_position(math::sine(1.0f, t) + off, math::cosine(0.6f, t), 0.0f);
	
	//glUseProgram(default_material->shader->id);
	//GLuint uniform_highlight = glGetUniformLocation(default_material->shader->id, "highlight");
	//if (collision(first, second)) {
	//	glUniform1f(uniform_highlight, 0.2f);
	//}
	//else {
	//	glUniform1f(uniform_highlight, 0.0f);
	//}
}

void World::draw() {
	//@TEMP
	Entity* first = get_entity(0);
	Entity* second = get_entity(1);

	Camera* cam = system.screen->get_camera();
	glm::mat4 transform_a = first->get_transform()->get_matrix();
	glm::mat4 transform_b = second->get_transform()->get_matrix();

	glm::mat4 projection = cam->projection_matrix();
	glm::mat4 view = cam->view_matrix();

	first->get_renderer()->draw(transform_a, view, projection);
	second->get_renderer()->draw(transform_b, view, projection);

	test_rend.draw();
}

void World::do_camera() {
	//@TEMP: Until scripting

	Camera* cam = system.screen->get_camera();
	Input* input = system.input;

	glm::vec3 cam_direction = { 0.0f, 0.0f, 0.0f };
	glm::vec3 cam_axis = { 0.0f, 1.0f, 0.0f };
	float cam_angle = 0.0f;

	if (input->get_key(SDLK_w)) cam_direction.z = -1.0f;
	if (input->get_key(SDLK_a)) cam_direction.x = -1.0f;
	if (input->get_key(SDLK_d)) cam_direction.x = 1.0f;
	if (input->get_key(SDLK_s)) cam_direction.z = 1.0f;

	if (glm::length(cam_direction) > 0.0f) {
		cam_direction = glm::normalize(cam_direction);
	}

	cam->transform_matrix() = glm::translate(cam->transform_matrix(), cam_direction * 0.05f);
	cam->transform_matrix() = glm::rotate(cam->transform_matrix(), -glm::radians(cam_angle), cam_axis);
}

Entity* World::get_entity(uint index) {
	if (index < entities.size()) {
		auto it = entities.begin();
		std::advance(it, index);
		return &(*it);
	}
	return nullptr;
}

Entity* World::create_entity(std::string name) {
	entities.emplace_back(name);
	return &entities.back();
}

Entity* World::copy_entity(Entity* ent) {
	entities.emplace_back(*ent);
	return &entities.back();
}

Entity* World::clone_entity(Entity* ent, glm::vec3 pos) {
	entities.emplace_back(*ent);
	entities.back().get_transform()->set_position(pos);
	return &entities.back();
}
Entity* World::clone_entity(Entity* ent, glm::vec3 pos, glm::quat rot) {
	entities.emplace_back(*ent);
	Transform* t = entities.back().get_transform();
	t->set_position(pos);
	t->set_rotation(rot);
	return &entities.back();
}
Entity* World::clone_entity(Entity* ent, glm::vec3 pos, glm::vec3 scale) {
	entities.emplace_back(*ent);
	Transform* t = entities.back().get_transform();
	t->set_position(pos);
	t->set_scale(scale);
	return &entities.back();
}
Entity* World::clone_entity(Entity* ent, glm::vec3 pos, glm::quat rot, glm::vec3 scale) {
	entities.emplace_back(*ent);
	Transform* t = entities.back().get_transform();
	t->set_position(pos);
	t->set_rotation(rot);
	t->set_scale(scale);
	return &entities.back();
}
Entity* World::clone_entity(Entity* ent, Transform* tfm) {
	entities.emplace_back(*ent);
	entities.back().set_transform(tfm);
	return &entities.back();
}

void World::destroy_entity(Entity* ent) {
	for (auto it = entities.begin(); it != entities.end();) {
		if (ent == &(*it)) {
			entities.erase(it);
			return;
		}
		else ++it;
	}
}

bool World::collision(Entity* a, Entity* b) {
	return a->get_collider()->intersect(b->get_collider());
}